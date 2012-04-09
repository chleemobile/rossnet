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

int set_dest(int current_location);

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}


void
init(airport_state * s, tw_lp * lp)
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
    
    LocalTrafficController *traffic_controller = new LocalTrafficController();
    s->traffic_controller = traffic_controller;

    for(i = 0; i < planes_per_airport; i++)
    {
        e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, 1), lp);
        m = (air_traffic_message*)tw_event_data(e);
        aircraft *airplane = (aircraft*)malloc(sizeof(aircraft));
        m->airplane = airplane;
        m->type = DEP_REQ;
        tw_event_send(e);
    }
    
    /*
     We initally set 10 region controllers
     LP gid 0 to 9 -> Region controller
     10 to 1023 -> Airport
     */
    if(lp->gid <10)
    {
        RegionTrafficController *region_controller = new RegionTrafficController();
        s->region_controller = region_controller;
    }
}

void
event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
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
            s->airplane = msg->airplane;
            if(s->traffic_controller->dep_req())
            {
                s->airplane->dest = set_dest(lp->gid);
                /*
                set route will be added later
                */
                ts =  s->traffic_controller->cal_dep_prep_time();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;

            }
            else
            {
                ts = s->traffic_controller->cal_delay();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            break;
        }
            
        case TAXI_OUT:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_taxi_out_time();
            e = tw_event_new(lp->gid, ts, lp);

            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REQ;
            m->airplane = msg->airplane;
            m->msg_from = lp->gid;
            tw_event_send(e);
            
            break;
        }
            
        /*
         Send a request to the region controller as an evetn since we have two different LPs
         */
        case TAKE_OFF_REQ:
        {
            s->airplane = msg->airplane;
            ts = 1; //should select a reasonable time
            int region_controller_id = 1; //should select a region controller
            e = tw_event_new(region_controller_id, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REP;
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            break;
        }

        /*
         Region Traffic Controller will receive this event
         */
        case TAKE_OFF_REP:
        {
            s->airplane = msg->airplane;
            if(s->region_controller->take_off_req())
            {
                ts = 1;
                e = tw_event_new(m->msg_from, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                
            }
            else
            {
                ts = s->traffic_controller->cal_delay();
                e = tw_event_new(m->msg_from, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            break;
        }            
            
        case TAKE_OFF:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_take_off_time();
            e = tw_event_new(s->airplane->dest, ts, lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            m->airplane = s->airplane;
            
            tw_event_send(e);
            break;
        }
            
        case ON_THE_AIR:
        {
            s->airplane = msg->airplane;
            /*
             update next sector will be added to here
             */
            int next_sector = 1;
            int dest = s->airplane->dest;
            if(next_sector == dest)
            {
                
            }
            else
            {
                //sector transit req will be added
                bool approved = true;
                if (approved) 
                {

                }
                else
                {
                    
                }
            }
            break;
        }
            
        case LANDING_REQ:
        {
            s->airplane = msg->airplane;
            if(s->traffic_controller->landing_req())
            {
                ts = (tw_stime)s->traffic_controller->cal_landing_prep_time();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
                
            }
            else
            {
                ts = s->traffic_controller->cal_delay();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
        } 
            
        case LANDING:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_landing_time();
            e = tw_event_new(lp->gid, ts, lp);
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL_REQ;
            m->airplane = s->airplane;
            
            tw_event_send(e);
            break;
        }   
            
        case ARRIVAL_REQ:
        {
            s->airplane = msg->airplane;
            if(s->traffic_controller->arrival_req())
            {
                ts =  s->traffic_controller->cal_arrival_prep_time();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_IN;
                
            }
            else
            {
                ts = s->traffic_controller->cal_delay();
                e = tw_event_new(lp->gid, ts, lp);
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ARRIVAL_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            break;
        }  
            
        case TAXI_IN:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_taxi_in_time();
            e = tw_event_new(lp->gid, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL;
            m->airplane = msg->airplane;
            
            tw_event_send(e);
            break;
        }  
            
        case ARRIVAL:
        {
            s->airplane = msg->airplane;
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
final(airport_state * s, tw_lp * lp)
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
		sizeof(airport_state),
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

int set_dest(int current_location)
{
    return 1;
}

int dep_req()
{
    return 1;
}

tw_stime cal_delay()
{
    return 1;
}
