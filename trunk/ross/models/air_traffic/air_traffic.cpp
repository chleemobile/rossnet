#include "air_traffic.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>
#include "LocalTrafficController.hpp"
#include "RegionTrafficController.hpp"
#include "Aircraft.hpp"

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
init(airport_state * s, tw_lp * lp)
{
    static int init_seed = 1;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
        RegionTrafficController *region_controller = new RegionTrafficController(RegionTrafficController::Large);
        s->region_controller = region_controller;
    }
    else
    {
        LocalTrafficController *traffic_controller;
        
        if (lp->gid == 20 ||
            lp->gid == 39 ||
            lp->gid == 73 ||
            lp->gid == 93 ||            
            lp->gid == 115||
            lp->gid == 149||            
            lp->gid == 167||
            lp->gid == 181||            
            lp->gid == 227||            
            lp->gid == 244||                        
            lp->gid == 319||                        
            (lp->gid >= 54 && lp->gid <= 56)   ||
            (lp->gid >= 202 && lp->gid <= 203) ||            
            (lp->gid >= 255 && lp->gid <= 256) ||                        
            (lp->gid >= 270 && lp->gid <= 272) ||    
            (lp->gid >= 295 && lp->gid <= 296) ||                
            (lp->gid >= 303 && lp->gid <= 305) ||                
            (lp->gid >= 338 && lp->gid <= 340) )
        {
            traffic_controller = new LocalTrafficController(LocalTrafficController::Large);
        }
        else if (lp->gid == 21 ||
                 lp->gid == 39 ||
                 lp->gid == 110 ||
                 lp->gid == 116 ||
                 lp->gid == 168 ||
                 lp->gid == 204 ||
                 lp->gid == 320 ||                 
                 (lp->gid >= 40 && lp->gid <= 43)   ||
                 (lp->gid >= 57 && lp->gid <= 59)   ||                 
                 (lp->gid >= 150 && lp->gid <= 152) ||
                 (lp->gid >= 182 && lp->gid <= 185) ||
                 (lp->gid >= 215 && lp->gid <= 216) ||                 
                 (lp->gid >= 228 && lp->gid <= 230) ||                                  
                 (lp->gid >= 245 && lp->gid <= 246) ||                 
                 (lp->gid >= 273 && lp->gid <= 275) ||                                  
                 (lp->gid >= 306 && lp->gid <= 307) ||                                                   
                 (lp->gid >= 341 && lp->gid <= 342))                                                                       
        {
            traffic_controller = new LocalTrafficController(LocalTrafficController::Medium);       
        }
        else if (lp->gid == 22 ||
                 lp->gid == 44 ||
                 lp->gid == 94 ||
                 lp->gid == 343||            
                 (lp->gid >= 60 && lp->gid <= 62)   ||
                 (lp->gid >= 74 && lp->gid <= 75) ||
                 (lp->gid >= 111 && lp->gid <= 112) ||
                 (lp->gid >= 117 && lp->gid <= 120) ||                                  
                 (lp->gid >= 153 && lp->gid <= 155) ||                                  
                 (lp->gid >= 169 && lp->gid <= 171) ||                 
                 (lp->gid >= 186 && lp->gid <= 188) ||                 
                 (lp->gid >= 205 && lp->gid <= 207) ||
                 (lp->gid >= 217 && lp->gid <= 219) ||                 
                 (lp->gid >= 231 && lp->gid <= 234) ||                 
                 (lp->gid >= 247 && lp->gid <= 250) ||                                  
                 (lp->gid >= 257 && lp->gid <= 261) ||                                  
                 (lp->gid >= 276 && lp->gid <= 281) ||                                                   
                 (lp->gid >= 297 && lp->gid <= 298) ||                                                   
                 (lp->gid >= 308 && lp->gid <= 310) ||                                                   
                 (lp->gid >= 321 && lp->gid <= 327))
        {
            traffic_controller = new LocalTrafficController(LocalTrafficController::Small); 
        }
        else
        {
            traffic_controller = new LocalTrafficController(LocalTrafficController::NH);
        }
        
        
        
        s->traffic_controller = traffic_controller;
        s->rn = init_seed++;
        
        for(i = 0; i < planes_per_airport; i++)
        {
            e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, 1), lp);
            m = (air_traffic_message*)tw_event_data(e);
            Aircraft *airplane = new Aircraft();
            m->airplane = airplane;
            m->type = DEP_REQ;
            tw_event_send(e);
        }
        
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
                int dest_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);
                int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1); 
                
                //                cout<<"airplane "<<s->airplane->get_id()
                //                    <<", dest airport: "<<dest_airport
                //                    <<", dest region : "<<dest_region<<endl;
                
                s->airplane->set_dest_region(dest_region);
                s->airplane->set_dest_airport(dest_airport);
                
                /*
                 set route will be added later
                 */
                
                ts =  s->traffic_controller->cal_dep_prep_time(s->rn, bs_rand_exponential);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                
                if(DEBUG)cout<<"Departure Accepted, airplane "<<s->airplane->get_id()<<" is from "<<lp->gid<<" to "<<s->airplane->get_dest_region()<<endl;
                
            }
            else
            {
                ts = s->traffic_controller->cal_delay(s->rn, bs_rand_exponential);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_REQ;
                
                if(DEBUG)cout<<"Departure Rejected, airplane "<<s->airplane->get_id()<<" is waiting "<<ts<< "times"<<endl;
                
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            
            break;
        }
            
        case TAXI_OUT:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_taxi_out_time(s->rn, bs_rand_integer);
            e = tw_event_new(lp->gid, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REQ;
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            if(DEBUG)cout<<"TAXI_OUT, airplane "<<s->airplane->get_id()<<" is at "<<lp->gid<<endl;
            
            break;
        }
            
            /*
             Send a request to the region controller since we have two different LPs
             */
        case TAKE_OFF_REQ:
        {
            s->airplane = msg->airplane;
            ts = 1; //should select a reasonable time
            int region_controller_id = s->airplane->get_next_region();
            e = tw_event_new(region_controller_id, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REP;
            m->airplane = msg->airplane;
            m->msg_from = lp->gid;
            
            tw_event_send(e);
            
            if(DEBUG)cout<<"TAKE_OFF_REQ, airplane "<<s->airplane->get_id()
                <<" at airport "<<lp->gid<<" sending a request to "<<region_controller_id<<endl;
            
            break;
        }
            
            /*
             Region controller will receive this event
             */
        case TAKE_OFF_REP:
        {
            s->airplane = msg->airplane;
            if(s->region_controller->take_off_req())
            {
                ts = 1;
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                
                if(DEBUG)cout<<"TAKE_OFF_REP Accepted, region controller "<<lp->gid<< " is receiving a request from "<<msg->msg_from<<endl;
            }
            else
            {
                ts = s->region_controller->cal_delay();
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF_REQ;
                if(DEBUG)cout<<"TAKE_OFF_REP Rejected, region controller "<<lp->gid<< " is receiving a request from "<<msg->msg_from<<endl;
                
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            
            break;
        }            
            
            /*
             Send an event to the region controller
             */
        case TAKE_OFF:
        {
            s->airplane = msg->airplane;
            ts = (tw_stime)s->traffic_controller->cal_take_off_time();
            int next_region = s->airplane->get_next_region();
            e = tw_event_new(next_region, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            m->airplane = s->airplane;
            
            tw_event_send(e);
            
            s->traffic_controller->free_runway();
            
            if(DEBUG)cout<<"TAKE_OFF, airplane "<<s->airplane->get_id()<<" is being handed off to region "<<next_region
                <<" from airport "<<lp->gid<<endl;
            
            break;
        }
            
            /*
             Region controller will receive this event
             */     
        case ON_THE_AIR:
        {
            s->airplane = msg->airplane;
            /*
             updating the next sector will be added to here
             */
            int next_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);
            
            if(next_region == s->airplane->get_dest_region())
            {
                ts = (tw_stime)s->region_controller->cal_flight_time();
                /*
                 send an event to airport 
                 */
                //cout<<"sending landing request to "<<s->airplane->get_dest_airport()<<endl;
                e = tw_event_new(s->airplane->get_dest_airport(), ts, lp);
                
                s->region_controller->hand_off();
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_REQ;
            }
            else
            {
                /*
                 Flight time to the next region
                 */
                ts = (tw_stime)s->region_controller->cal_flight_time();
                e = tw_event_new(next_region, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TRANSIT_REQ;
                m->msg_from = lp->gid;
            }
            m->airplane = s->airplane;
            tw_event_send(e);
            
            if(DEBUG)cout<<"ON_THE_AIR, airplane "<<s->airplane->get_id()<<" is at region "<<lp->gid<<"eid "<<e->event_id<<endl;
            
            break;
        }
            
            /*
             Region controller will receive this event
             */
        case TRANSIT_REQ:
        {
            s->airplane = msg->airplane;
            if(s->region_controller->transit_req())
            {
                ts = s->region_controller->cal_transit_time();
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = HAND_OFF;
                m->msg_from = lp->gid;
                
                if(DEBUG)cout<<"TRANSIT_REQ Accepted, airplane "<<s->airplane->get_id()<<" is at region "<<lp->gid<<"handing off to "<<endl;
            }
            else
            {
                ts = s->region_controller->cal_delay();
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ON_THE_AIR;
                //should re-update the next region updated in ON_THE_AIR event 
                
                if(DEBUG)cout<<"TRANSIT_REQ Rejected, airplane "<<s->airplane->get_id()<<" is at region "<<endl;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            
            break;
        }             
            
        case HAND_OFF:
        {
            s->airplane = msg->airplane;
            ts = 1;
            e = tw_event_new(msg->msg_from, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            m->airplane = s->airplane;
            
            tw_event_send(e);
            
            s->region_controller->hand_off();
            if(DEBUG)cout<<"HAND_OFF"<<endl;
            
            break;
        }
            
        case LANDING_REQ:
        {
            s->airplane = msg->airplane;
            if(s->traffic_controller->landing_req())
            {
                ts = (tw_stime)s->traffic_controller->cal_landing_prep_time();
                //ts = bs_rand_exponential(s->rn, 5);
                //cout<<"ts"<<ts<<endl;
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
            }
            else
            {
                ts = s->traffic_controller->cal_delay(s->rn, bs_rand_exponential);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            if(DEBUG)cout<<"LANDING_REQ"<<endl;            
            
            break;
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
            if(DEBUG)cout<<"LANDING, aiport "<<lp->gid<<"from"<<msg->msg_from<<"eid "<<e->event_id<<endl;            
            s->traffic_controller->free_runway();
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
                ts = s->traffic_controller->cal_delay(s->rn, bs_rand_exponential);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ARRIVAL_REQ;
            }
            m->airplane = msg->airplane;
            tw_event_send(e);
            if(DEBUG)cout<<"ARRIVAL_REQ"<<endl;            
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
            if(DEBUG)cout<<"TAXI_IN"<<endl;            
            
            break;
        }  
            
        case ARRIVAL:
        {
            s->airplane = msg->airplane;
            if(DEBUG)cout<<"ARRIVAL, airplane "<<s->airplane->get_id()<<" is at airport "<<lp->gid<<endl;
            
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
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
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
