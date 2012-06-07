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

void event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    tw_stime ts;
    tw_event *e;
    air_traffic_message *m;
    
    switch(msg->type)
    {
        * (int *)bf = (int)0;
            
        case DEP:
        {
            if (s->runway_in_use < s->max_runway) 
            {
                bf->c1=0;
                s->runway_in_use++;
				s->dep_req_accepted++;

				//dummy loop
				int i=0;
				while(i < s->max_runway)
				{
					s->arrival_req_accepted++;
					i++;
				}

                int dest_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);
                e = tw_event_new(dest_region, bs_rand_exponential(s->rn, 1), lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                m->dest_region = dest_region;
                tw_event_send(e);

            }
            else
            {
                bf->c1=1;
				s->dep_req_rejected++;

            }

            break;
        }

        case TAKE_OFF:
        {            
            s->runway_in_use--;

            int dest = bs_rand_integer(s->rn, 0, NUMBER_OF_LP-1);
            e = tw_event_new(dest, bs_rand_exponential(s->rn, 10), lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_AIR;
            m->dest_region = msg->dest_region;
            tw_event_send(e);

            break;
        }
            
        case ON_AIR:
        {
            int dest = bs_rand_integer(s->rn, 0, NUMBER_OF_LP-1);
            e = tw_event_new(dest, bs_rand_exponential(s->rn, 10), lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP;
            m->dest_region = msg->dest_region;

            tw_event_send(e);
            break;
        }
    }
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
            m->type = DEP;            
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
        * (int *)bf = (int)0;
            
        case DEP:
        {
            if (s->runway_in_use < s->max_runway) 
            {
                bf->c1=0;
                s->runway_in_use++;
                int dest_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp);
                e = tw_event_new(dest_region, bs_rand_exponential2(s->rn, 1, lp)+1, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                m->dest_region = dest_region;
                tw_event_send(e);

            }
            else
            {
                bf->c1=1;

            }

            break;
        }

        case TAKE_OFF:
        {            
            s->runway_in_use--;

            int dest = bs_rand_integer2(s->rn, 0, NUMBER_OF_LP-1, lp);
            e = tw_event_new(dest, bs_rand_exponential2(s->rn, 10, lp)+1, lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_AIR;
            m->dest_region = msg->dest_region;
            tw_event_send(e);

            break;
        }
            
        case ON_AIR:
        {
            int dest = bs_rand_integer2(s->rn, 0, NUMBER_OF_LP-1, lp);
            e = tw_event_new(dest, bs_rand_exponential2(s->rn, 10, lp)+1, lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP;
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
        case DEP:
        {
            if(bf->c1=0)
            {
                s->runway_in_use--;
                bs_rand_rvs(s->rn, lp);
                bs_rand_rvs(s->rn, lp);
            }
            else
            {
            }

            break;
        }
        case TAKE_OFF:
        {
            s->runway_in_use++;

            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);

            break;
        }    
        case ON_AIR:
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
		(event_f) event_handler,
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

