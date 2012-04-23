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
        s->max_capacity = 200;
        s->airplane_in_region = 0;
    }
    else
    {
        
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
            s->max_runway = 6;
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
            s->max_runway = 3;
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
            s->max_runway = 2;
        }
        else
        {
            s->max_runway = 1;
            
        }
        
        s->rn = init_seed++;
        s->runway_in_use = 0;
        
        for(i = 0; i < planes_per_airport; i++)
        {
            e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, 1), lp);
            m = (air_traffic_message*)tw_event_data(e);
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
            if(s->runway_in_use < s->max_runway)
            {
                s->runway_in_use++;
                assert(s->runway_in_use <= s->max_runway);
                
                
                int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1); 
                int source_region = get_region(lp->gid);
                int dest_region = get_region(dest_airport);
                
                assert(source_region >=0);
                assert(dest_region <=19);
                
                int next_region = 0;
                deque<int> p = graph->get_shortest_path(source_region, dest_region);
                
                if (p.size() == 1) 
                {
                    next_region = dest_region;
                }
                else
                {
                    p.back();
                    next_region = p.back();
                }
                
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                m->dest_region = dest_region ;
                m->dest_airport = dest_airport;
                m->next_region = next_region;
            }
            else
            {
                //departure request delay
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_REQ;
                
            }
            tw_event_send(e);
            
            break;
        }
            
        case TAXI_OUT:
        {
            //Taxi out time
            ts = bs_rand_exponential(s->rn, 1);
            e = tw_event_new(lp->gid, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REQ;
            m->dest_region = msg->dest_region ;
            m->dest_airport = msg->dest_airport;
            m->next_region = msg->next_region;
            
            tw_event_send(e);
            
            break;
        }
            
            /*
             Send a request to the region controller since we have two different LPs
             */
        case TAKE_OFF_REQ:
        {
            ts = bs_rand_exponential(s->rn, 1);
            int next_region = msg->next_region;
            e = tw_event_new(next_region, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF_REP;
            m->dest_region = msg->dest_region ;
            m->dest_airport = msg->dest_airport;
            m->msg_from = lp->gid;
            
            tw_event_send(e);
            
            break;
        }
            
            /*
             Region controller will receive this event
             */
        case TAKE_OFF_REP:
        {
            if(s->airplane_in_region < s->max_capacity)
            {
                s->airplane_in_region++;
                assert(s->airplane_in_region <= s->max_capacity);
                
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF;
                m->dest_region = msg->dest_region ;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
                
            }
            else
            {
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAKE_OFF_REQ;
                m->dest_region = msg->dest_region ;
                m->dest_airport = msg->dest_airport;
                m->next_region = msg->next_region;
            }
            
            tw_event_send(e);
            
            break;
        }            
            
            /*
             Send an event to the region controller
             */
        case TAKE_OFF:
        {
            ts = bs_rand_exponential(s->rn, 1);
            int next_region = msg->msg_from;
            e = tw_event_new(next_region, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            m->dest_region = msg->dest_region ;
            m->dest_airport = msg->dest_airport;
            
            tw_event_send(e);
            
            s->runway_in_use--;
            assert(s->runway_in_use >= 0);
            
            break;
        }
            
            /*
             Region controller will receive this event
             */     
        case ON_THE_AIR:
        {
            
            int next_region = 0;
            deque<int> p = graph->get_shortest_path(lp->gid, msg->dest_region);
            
            if (p.size() == 1) 
            {
                next_region = msg->dest_region;
            }
            else
            {
                p.back();
                next_region = p.back();
            }
            
//            int next_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);
            
            if(next_region == msg->dest_region)
            {
                ts = bs_rand_exponential(s->rn, 1);
                /*
                 send an event to airport 
                 */
                e = tw_event_new(msg->dest_airport, ts, lp);
                
                s->airplane_in_region--;
                assert(s->airplane_in_region >= 0);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_REQ;
            }
            else
            {
                /*
                 Flight time to the next region
                 */
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(next_region, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TRANSIT_REQ;
                m->dest_region = msg->dest_region;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
            }
            tw_event_send(e);
            
            break;
        }
            
            /*
             Region controller will receive this event
             */
        case TRANSIT_REQ:
        {
            if(s->airplane_in_region < s->max_capacity)
            {
                s->airplane_in_region++;
                assert(s->airplane_in_region <= s->max_capacity);
                
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = HAND_OFF;
                m->dest_region = msg->dest_region ;
                m->dest_airport = msg->dest_airport;
                m->msg_from = lp->gid;
                
            }
            else
            {
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(msg->msg_from, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ON_THE_AIR;
                m->dest_region = msg->dest_region ;
                m->dest_airport = msg->dest_airport;
                //should re-update the next region updated in ON_THE_AIR event 
                
            }
            tw_event_send(e);
            
            break;
        }             
            
        case HAND_OFF:
        {
            ts = bs_rand_exponential(s->rn, 1);
            e = tw_event_new(msg->msg_from, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ON_THE_AIR;
            m->dest_region = msg->dest_region ;
            m->dest_airport = msg->dest_airport;
            
            s->airplane_in_region--;
            assert(s->airplane_in_region >= 0);
            
            tw_event_send(e);
            
            break;
        }
            
        case LANDING_REQ:
        {
            if(s->runway_in_use < s->max_runway)
            {
                s->runway_in_use++;
                assert(s->runway_in_use <= s->max_runway);
                
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
            }
            else
            {
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_REQ;
            }
            tw_event_send(e);
            
            break;
        } 
            
        case LANDING:
        {
            ts = bs_rand_exponential(s->rn, 1);
            e = tw_event_new(lp->gid, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL_REQ;
            
            tw_event_send(e);
            
            s->runway_in_use--;
            assert(s->runway_in_use >= 0);
            
            break;
        }   
            
        case ARRIVAL_REQ:
        {
            if(s->runway_in_use < s->max_runway)
            {
                s->runway_in_use++;
                assert(s->runway_in_use <= s->max_runway);
                
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_IN;
                
            }
            else
            {
                ts = bs_rand_exponential(s->rn, 1);
                e = tw_event_new(lp->gid, ts, lp);
                
                m = (air_traffic_message*)tw_event_data(e);
                m->type = ARRIVAL_REQ;
            }
            tw_event_send(e);
            break;
        }  
            
        case TAXI_IN:
        {
            ts = bs_rand_exponential(s->rn, 1);
            e = tw_event_new(lp->gid, ts, lp);
            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL;
            
            tw_event_send(e);
            
            break;
        }  
            
        case ARRIVAL:
        {
            s->runway_in_use--;
            assert(s->runway_in_use >= 0);
            
            if(DEBUG)cout<<"ARRIVAL" <<lp->gid<<endl;
            
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
