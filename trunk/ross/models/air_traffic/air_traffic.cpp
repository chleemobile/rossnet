#include "air_traffic.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>

/*
 Air_Traffic.cpp
 Air Traffic Simulator
 20120406
 Chayong Lee
 
 2008/2/16
 Modified for ROSS 4.0
 David Bauer
 */

/*
 This is a simple version using a graph to run a path find algorithm!
 */
int get_region(int airport);

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}


void
p_init(airport_state * s, tw_lp * lp)
{
    static int init_seed = 1;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
    s->rn=0;
    s->from=0;
    s->dest_airport = 0;
    s->dest_region = 0;
    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
        s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;

    }
    else
    {
        s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
        
        s->runway_in_use=0;
        
        s->landing=0;
        s->arrival_req_accepted=0;
        s->arrival_req_rejected=0;
        s->dep_req_accepted=0;
        s->dep_req_rejected=0;
        
        for(i = 0; i < planes_per_airport; i++)
        {
            e = tw_event_new(lp->gid, bs_rand_exponential2(s->rn, 1, lp), lp);
            m = (air_traffic_message*)tw_event_data(e);

            m->type = TAKE_OFF_REQ;       
            m->dest_airport = -1;
            m->dest_region = -1;
            m->msg_from = -1;
            
            tw_event_send(e);
        }
        
    }
}

void
fw_event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    tw_stime ts;
    tw_event *e;
    air_traffic_message *m;
    
    switch(msg->type)
    {
        case TAKE_OFF_REQ:
        {            
            assert(msg->dest_region < NUMBER_OF_REGION_CONTROLLER);
            
            int dest_airport = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp); 
            int dest_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp); 

            s->dest_airport = dest_airport;
            s->dest_region = dest_region;
            s->from = lp->gid;
            
            e = tw_event_new(s->dest_region, bs_rand_exponential2(s->rn, 1, lp), lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REP;
            
            m->msg_from = s->from;
            m->dest_region = s->dest_region;
            m->dest_airport = s->dest_airport;

            tw_event_send(e);

            break;
        }

        case TAKE_OFF_REP:
        {                        
            assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
            
            s->dest_region = msg->dest_region;
            s->dest_airport = msg->dest_airport;
            s->from = msg->msg_from;
            
            int rand = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1,lp);

            if(lp->gid == rand)
            {
                bf->c1=0;
                e = tw_event_new(s->from, bs_rand_exponential2(s->rn, 10, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            else
            {
                bf->c2=0;
                e = tw_event_new(s->from, bs_rand_exponential2(s->rn, 5, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF_REQ;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            
            tw_event_send(e);

            break;
        }
            
        case TAKE_OFF:
        {

//            e = tw_event_new(msg->msg_from, bs_rand_exponential2(s->rn, 1, lp), lp);
//            m = (air_traffic_message*)tw_event_data(e);
//            m->type = ON_THE_AIR;
//            m->msg_from = lp->gid;
//            m->dest_region = msg->dest_region;
//            m->dest_airport = msg->dest_airport;
//
////            tw_event_send(e);

            break;
        }
            
        case ON_THE_AIR:
        {           
            assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
            int rand = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1,lp);
            if(rand == msg->dest_region)
            {
                bf->c1=0;
                e = tw_event_new(msg->dest_airport, bs_rand_exponential2(s->rn, 10, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LAND;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            else
            {
                bf->c2=0;
                
                int next = bs_rand_integer2(s->rn, 0,NUMBER_OF_REGION_CONTROLLER-1,lp);

                e = tw_event_new(next, bs_rand_exponential2(s->rn, 5, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = HAND_OFF_REQ;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            
            tw_event_send(e);
            
            break;
        }
            
        case HAND_OFF_REQ:
        {
            //cout<<"HAND_OFF_REQ"<<endl;
            
            assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
            
            int result = bs_rand_integer2(s->rn, 0,2,lp);
            
            if(result == 0)//accepted
            {
                bf->c1=0;
                e = tw_event_new(msg->msg_from, bs_rand_exponential2(s->rn, 100, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = HAND_OFF;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            else
            {
                bf->c2=0;
                e = tw_event_new(msg->msg_from, bs_rand_exponential2(s->rn, 10, lp), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ON_THE_AIR;
                
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            
            tw_event_send(e);

            break;
        }
            
        case HAND_OFF:
        {
            //cout<<"HAND_OFF"<<endl;

            e = tw_event_new(msg->msg_from, bs_rand_exponential2(s->rn, 10, lp), lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            
            m->dest_region = msg->dest_region;
            m->dest_airport = msg->dest_airport;
            m->msg_from = lp->gid;
            
            tw_event_send(e);

            break;
        }
              
        case LAND:
        {
           cout<<"LAND"<<endl;

            break;
        }
    }
}


void
rc_event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    switch(msg->type)
    {
        case TAKE_OFF_REQ:
        {
//            cout<<msg->dest_region<<endl;
//            assert(msg->dest_region == s->dest_region);
            
//            msg->dest_region = s->dest_region;
//            msg->dest_airport = s->dest_airport;
//            msg->msg_from = s->from;

            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);

            break;
        }
        case TAKE_OFF_REP:
        {
//            assert(msg->dest_region == s->dest_region);
//            msg->dest_region = s->dest_region;
//            msg->dest_airport = s->dest_airport;
//            msg->msg_from = s->from;
            
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);

            break;
        }    
        case TAKE_OFF:
        {

//            bs_rand_rvs(s->rn, lp);

            break;
        } 
        case ON_THE_AIR:
        {
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
            
            break;
        }       
            
        case HAND_OFF_REQ:
        {
            assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);

            break;
        } 
            
        case HAND_OFF:
        {
            bs_rand_rvs(s->rn, lp);
            
            break;
        }      
            
        case LAND:
        {
            break;
        }
    }
}

void
final(airport_state * s, tw_lp * lp)

{
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
    total_transit_accepted += s->transit_req_accepted;
    total_transit_rejected += s->transit_req_rejected;
    
    total_dep_req_accepted += s->dep_req_accepted;
    total_dep_req_rejected += s->dep_req_rejected;
    
    total_arrival_req_accepted += s->arrival_req_accepted;
    total_arrival_req_rejected += s->arrival_req_rejected;
    
//    cout<<lp->gid<<","<<s->arrival_req_accepted<<endl;
}

/*
 Parallel Running
 */

tw_lptype airport_lps[] =
{
	{
		(init_f) p_init,
		(event_f) fw_event_handler,
		(revent_f) rc_event_handler,
		(final_f) final,
		(map_f) mapping,
		sizeof(airport_state),
	},
	{0},
};

/*
 Sequential Running
 */
//tw_lptype airport_lps[] =
//{
//	{
//		(init_f) init,
//		(event_f) event_handler,
//		(revent_f) rc_event_handler,
//		(final_f) final,
//		(map_f) mapping,
//		sizeof(airport_state),
//	},
//	{0},
//};


const tw_optdef app_opt [] =
{
	TWOPT_GROUP("Airport Model"),
	//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
	TWOPT_UINT("nplanes", planes_per_airport, "initial # of planes per airport(events)"),
	TWOPT_STIME("mean", mean_flight_time, "mean flight time for planes"),
	TWOPT_UINT("memory", opt_mem, "optimistic memory"),
	TWOPT_END()
};

int
main(int argc, char **argv, char **env)
{
	int i;
    
	tw_opt_add(app_opt);
	tw_init(&argc, &argv);
    
	nlp_per_pe /= (tw_nnodes() * g_tw_npe);
    
    cout<<"tw_nnode "<<tw_nnodes()<<endl;
    cout<<"g_tw "<<g_tw_npe<<endl;
    
	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(air_traffic_message), 0);
    
    /*
     init graph
     */
    
    graph = new Graph(20);
    graph->create_graph(GRAPH_CSV_FILE_PATH);
    /*
     We have two different LPs
     One represents an airport
     The other one represents Region Controller
     */
	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &airport_lps[0]);
    
	tw_run();
    
	if(tw_ismaster())
	{
		printf("\nAir Traffic Model Statistics:\n");
        
        printf("\t%-50s %11lld\n", "Number of LPs (Airport, Region Controller)", 
               nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
               planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());
        
        cout<<"\tTotal Transit Accepted : "<<total_transit_accepted<<endl;
        cout<<"\tTotal Transit Rejected : "<<total_transit_rejected<<endl;
        
        cout<<"\tTotal Departure Req Accepted : "<<total_dep_req_accepted<<endl;
        cout<<"\tTotal Departure Req Rejected : "<<total_dep_req_rejected<<endl;
        
        cout<<"\tTotal Arrival Req Accepted : "<<total_arrival_req_accepted<<endl;
        cout<<"\tTotal Arrival Req Rejected : "<<total_arrival_req_rejected<<endl;
        
        
	}
    
	tw_end();
	
	if(MEM_USAGE)
	{
        //	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		cout<<memusage<<","<<endl;
	}	
    
	return 0;
}

int get_region(int airport)
{
    if(airport >=20 && airport <= 38)
    {
        return 0;
    }
    else if(airport >=39 && airport <= 53)
    {
        return 1;
    }
    else if(airport >=54 && airport <= 72)
    {
        return 2;
    }
    else if(airport >=73 && airport <= 92)
    {
        return 3;
    }
    else if(airport >=93 && airport <= 109)
    {
        return 4;
    }    
    else if(airport >=110 && airport <= 114)
    {
        return 5;
    }
    else if(airport >=115 && airport <= 148)
    {
        return 6;
    }    
    else if(airport >=149 && airport <= 166)
    {
        return 7;
    }    
    else if(airport >=167 && airport <= 180)
    {
        return 8;
    }
    else if(airport >=181 && airport <= 201)
    {
        return 9;
    }
    else if(airport >=202 && airport <= 214)
    {
        return 10;
    }
    else if(airport >=215 && airport <= 226)
    {
        return 11;
    }
    else if(airport >=227 && airport <= 243)
    {
        return 12;
    }    
    else if(airport >=244 && airport <= 254)
    {
        return 13;
    }    
    else if(airport >=255 && airport <= 269)
    {
        return 14;
    }    
    else if(airport >=270 && airport <= 294)
    {
        return 15;
    }    
    else if(airport >=295 && airport <= 302)
    {
        return 16;
    }    
    else if(airport >=303 && airport <= 318)
    {
        return 17;
    }        
    else if(airport >=319 && airport <= 337)
    {
        return 18;
    }    
    else if(airport >=338 && airport <= 347)
    {
        return 19;
    }   
    else
    {
        cout<<"airport region failed"<<endl;
        assert(false);
    }
    
}
