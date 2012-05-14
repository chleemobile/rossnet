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
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
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
			tw_stime ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
            e = tw_event_new(lp->gid, ts, lp);            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;            
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
//        *(int *)bf = (int)0;
            
        case DEP_REQ:
        {
			assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
			
			int weather = bs_rand_integer2(s->rn, 0, 3, lp);
			
			int path = 0;
			
            if ((path = (s->runway_in_use < s->max_runway))) 
            {
                s->runway_in_use++;
                s->dep_req_accepted++;
				
				int dest_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp);

				ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                m->dest_region = dest_region;
				
				tw_event_send(e);


            }
            else
            {
				s->dep_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				ts += weather;

                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_DELAY;
				m->msg_from = lp->gid;

				tw_event_send(e);

            }

			__store__(path,lp);
			
            break;
        }

        case DEP_DELAY:
        {
            
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
            tw_event_send(e);

            break;
        }
            
		case TAXI_OUT:
		{
			assert(msg->dest_region < NUMBER_OF_REGION_CONTROLLER);
			
			ts = bs_rand_exponential2(s->rn, MEAN_TAXI, lp);
			
            e = tw_event_new(msg->dest_region, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF;
			m->dest_region = msg->dest_region;
            tw_event_send(e);
			break;
		}
			
		case TAKE_OFF_REQ:
		{
			
			
			break;
		}

        case TAKE_OFF:
        {            
            s->runway_in_use--;
			
			int dest = bs_rand_integer2(s->rn, 0, NUMBER_OF_LP-1, lp);
			
			ts = bs_rand_exponential2(s->rn, MEAN_TAKE_OFF, lp);
			
            
            e = tw_event_new(dest, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = LAND;
            m->dest_region = msg->dest_region;
			
            tw_event_send(e);

            break;
        }
            
        case LAND:
        {
			int dest = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
			
			ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
			
            e = tw_event_new(dest, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
            m->dest_region = msg->dest_region;
			
            tw_event_send(e);
            
            break;
        }
                
    }
}


void
rc_event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    switch(msg->type)
    {
        case DEP_REQ:
        {
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);

			if (path) {
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);

                s->dep_req_accepted--;
				s->runway_in_use--;
			}
			else {

				bs_rand_rvs(s->rn, lp);

				s->dep_req_rejected--;
			}

			bs_rand_rvs(s->rn, lp);

            break;
        }
            
        case DEP_DELAY:
        {
            bs_rand_rvs(s->rn, lp);
            
            break;
        }

        case TAXI_OUT:
        {
            bs_rand_rvs(s->rn, lp);
            
            break;
        }
            
        case TAKE_OFF:
        {
            s->runway_in_use++;

            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);

            break;
        }  
            
        case LAND:
        {
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
            
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
    
    //graph = new Graph(20);
    //graph->create_graph(GRAPH_CSV_FILE_PATH);
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

