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

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}


void
init(air_traffic_state * s, tw_lp * lp)
{
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
    s->landings = 0;
    s->planes_in_the_sky = 0;
    s->planes_on_the_ground = planes_per_airport;
    s->waiting_time = 0.0;
    s->furthest_flight_landing = 0.0;
    
    for(i = 0; i < planes_per_airport; i++)
    {
        e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, MEAN_DEPARTURE), lp);
//        e = tw_event_new(lp->gid, bs_rand_exponential2(s->rn, MEAN_DEPARTURE, lp), lp);

        m = (air_traffic_message*)tw_event_data(e);
        m->type = DEP_REQ;
        tw_event_send(e);
    }
}

void
event_handler(air_traffic_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    int rand_result;
    tw_lpid dst_lp;
    tw_stime ts;
    tw_event *e;
    air_traffic_message *m;
    
    switch(msg->type)
    {
        case DEP_REQ:
        {

            break;
        }
            
        case TAXI_OUT:
        {

            break;
        }
            
        case TAKE_OFF_REQ:
        {

            break;
        }

        case TAKE_OFF:
        {
            
            break;
        }
            
        case ON_THE_AIR:
        {
            
            break;
        }
            
        case LANDING_REQ:
        {
            
            break;
        } 
            
        case LANDING:
        {
            
            break;
        }   
            
        case ARRIVAL_REQ:
        {
            
            break;
        }  
            
        case TAXI_IN:
        {
            
            break;
        }  
            
        case ARRIVAL:
        {
            
            break;
        }              
    }
}

void
rc_event_handler(air_traffic_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    switch(msg->type)
    {
        case DEP_REQ:
        {
            
            break;
        }
            
        case TAXI_OUT:
        {
            
            break;
        }
            
        case TAKE_OFF_REQ:
        {
            
            break;
        }
            
        case TAKE_OFF:
        {
            
            break;
        }
            
        case ON_THE_AIR:
        {
            
            break;
        }
            
        case LANDING_REQ:
        {
            
            break;
        } 
            
        case LANDING:
        {
            
            break;
        }  
            
        case ARRIVAL_REQ:
        {
            
            break;
        }  
            
        case TAXI_IN:
        {
            
            break;
        }  
            
        case ARRIVAL:
        {
            
            break;
        }              
    }
}

void
final(air_traffic_state * s, tw_lp * lp)
{
	wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
}

/*
 Sequential Running
 */


tw_lptype airport_lps[] =
{
	{
		(init_f) init,
		(event_f) event_handler,
		(revent_f) rc_event_handler,
		(final_f) final,
		(map_f) mapping,
		sizeof(air_traffic_state),
	},
	{0},
};


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
	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(air_traffic_message), 0);

	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &airport_lps[0]);

	tw_run();

	if(tw_ismaster())
	{
		printf("\nAirport Model Statistics:\n");
		printf("\t%-50s %11.4lf\n", "Average Waiting Time", wait_time_avg);
		printf("\t%-50s %11lld\n", "Number of airports", 
			nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
			planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());
	}

	tw_end();
	
	if(MEM_USAGE)
	{
	//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		cout<<memusage<<","<<endl;
	}	

	return 0;
}
