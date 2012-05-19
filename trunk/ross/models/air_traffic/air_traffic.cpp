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
    
    if (gid == 0) {
        return 0;
    }
    else if (gid == 1)
    {
        return 0;
    }
    else if(gid ==100)
        return 0;
    else if(gid ==101)
        return 0;
    
    if(gid < 174)
        return 0;
    else 
        return 1;
}

void
init(airport_state * s, tw_lp * lp)
{
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
	s->rn=lp->gid;
    
    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;			
		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;			
		}
		else {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;			
		}
        
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;
		
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
            s->max_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
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
            s->max_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
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
            s->max_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
        }
        else
        {
            s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
            
        }
		
        
        s->runway_in_use=0;
        
        s->landing=0;
		
        s->landing_req_accepted=0;
        s->landing_req_rejected=0;
        s->dep_req_accepted=0;
        s->dep_req_rejected=0;
        
        for(i = 0; i < planes_per_airport; i++)
        {
			tw_stime ts = bs_rand_exponential(s->rn, MEAN_DEQ);
            e = tw_event_new(lp->gid, ts, lp);            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;            
            tw_event_send(e);
        }
        
    }
}


void
event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
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
			
			int weather = bs_rand_integer(s->rn, 0, 3);
			
            if (s->runway_in_use < s->max_runway) 
            {
                s->runway_in_use++;
                s->dep_req_accepted++;
				
				
				int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
				int dest_region = get_region(dest_airport);
				
				ts = bs_rand_exponential(s->rn, MEAN_DEQ);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                m->dest_region = dest_region;
				m->dest_airport = dest_airport;
				
				tw_event_send(e);
				
				
            }
            else
            {
				s->dep_req_rejected++;
				
				ts = bs_rand_exponential(s->rn, MEAN_DELAY);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_DELAY;
				m->msg_from = lp->gid;
				
				tw_event_send(e);
				
            }
			
            break;
        }
			
        case DEP_DELAY:
        {
            
			ts = bs_rand_exponential(s->rn, MEAN_DELAY);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
            tw_event_send(e);
			
            break;
        }
            
		case TAXI_OUT:
		{
			assert(msg->dest_region < NUMBER_OF_REGION_CONTROLLER);
			
			ts = bs_rand_exponential(s->rn, MEAN_TAXI);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
			break;
		}
			
        case TAKE_OFF:
        {            
            s->runway_in_use--;
			
			
			int src_region = get_region(lp->gid);
			int next_region =-1;
			deque<int> p = graph->get_shortest_path(src_region, msg->dest_region);
			
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			
			ts = bs_rand_exponential(s->rn, MEAN_TAKE_OFF);
            
            e = tw_event_new(next_region, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
            m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
            break;
        }
            
		case TRANSIT_REQ:
		{
			
            if (s->airplane_in_region < s->max_capacity)
			{
                
				s->airplane_in_region++;
				s->transit_req_accepted++;
				
				ts = bs_rand_exponential(s->rn, MEAN_FLIGHT);
				
				e = tw_event_new(lp->gid, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = ON_THE_AIR;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
			}
			else
			{
				s->transit_req_rejected++;
				
				ts = bs_rand_exponential(s->rn, MEAN_DELAY);
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TRANSIT_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
			}
			
            tw_event_send(e);
            
			break;
		}
			
		case ON_THE_AIR:
		{
			assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
			
			int next_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);
			
			deque<int> p = graph->get_shortest_path(lp->gid, msg->dest_region);
			
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			s->airplane_in_region--;
			
            if (next_region == msg->dest_region)
			{
				ts = bs_rand_exponential(s->rn, MEAN_LAND);
				e = tw_event_new(msg->dest_airport, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING_REQ;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
			}
			else
			{
				ts = bs_rand_exponential(s->rn, MEAN_REQ);
				
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
			
		case TRANSIT_DELAY:
		{
			ts = bs_rand_exponential(s->rn, MEAN_DELAY);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
        case LANDING_REQ:
        {
			int weather = bs_rand_integer(s->rn, 0, 3);
            
            if (s->runway_in_use < s->max_runway)
            {
                s->runway_in_use++;
                s->landing_req_accepted++;
				
				ts = bs_rand_exponential(s->rn, MEAN_LAND);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
            }
            else
            {
				s->landing_req_rejected++;
				
				ts = bs_rand_exponential(s->rn, MEAN_DELAY);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
            }
			
			tw_event_send(e);
            
            break;
        }
			
		case LANDING_DELAY:
		{	
			ts = bs_rand_exponential(s->rn, MEAN_DELAY);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = LANDING_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case LANDING:
		{
			ts = bs_rand_exponential(s->rn, MEAN_TAXI);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAXI_IN;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}	
			
		case TAXI_IN:
		{
			ts = bs_rand_exponential(s->rn, MEAN_ARRIVAL);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case ARRIVAL:
		{
			s->runway_in_use--;
			
			int dest = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
			
			ts = bs_rand_exponential(s->rn, MEAN_LAND);
			
            e = tw_event_new(dest, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
			
            tw_event_send(e);
            
            break;
		}
			
    }
}




/*******************
 Parallel
 ******************/

void
p_init(airport_state * s, tw_lp * lp)
{
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
	s->rn=lp->gid;
    
    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;			
		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;			
		}
		else {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;			
		}
		
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;
		
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
            s->max_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
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
            s->max_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
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
            s->max_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
        }
        else
        {
            s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
            
        }
		
        s->runway_in_use=0;
        
        s->landing=0;
		
        s->landing_req_accepted=0;
        s->landing_req_rejected=0;
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
				__store__(s->runway_in_use, lp);
				__store__(s->dep_req_accepted, lp);
				
                s->runway_in_use++;
                s->dep_req_accepted++;
				
				
				int dest_airport = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
				int dest_region = get_region(dest_airport);
				
				ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                m->dest_region = dest_region;
				m->dest_airport = dest_airport;
				
				tw_event_send(e);
				
				
            }
            else
            {
				__store__(s->dep_req_rejected, lp);
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
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
			break;
		}
			
        case TAKE_OFF:
        {            
			__store__(s->runway_in_use, lp);
            s->runway_in_use--;
			
			
			int src_region = get_region(lp->gid);
			int next_region =-1;
			deque<int> p = graph->get_shortest_path(src_region, msg->dest_region);
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			
			ts = bs_rand_exponential2(s->rn, MEAN_TAKE_OFF, lp);
            
            e = tw_event_new(next_region, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
            m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
            break;
        }
            
		case TRANSIT_REQ:
		{
			int path = 0;
			
            if ((path = (s->airplane_in_region < s->max_capacity)))
			{
				
				__store__(s->airplane_in_region, lp);
				__store__(s->transit_req_accepted, lp);
				
				s->airplane_in_region++;
				s->transit_req_accepted++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT, lp);
				
				e = tw_event_new(lp->gid, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = ON_THE_AIR;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
			}
			else
			{
				__store__(s->transit_req_rejected, lp);
				s->transit_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TRANSIT_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
			}
			
            tw_event_send(e);
			
			__store__(path,lp);
			
			break;
		}
			
		case ON_THE_AIR:
		{
			assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
			
			int next_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp);
			
			deque<int> p = graph->get_shortest_path(lp->gid, msg->dest_region);
			
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			__store__(s->airplane_in_region, lp);
			s->airplane_in_region--;
			
			int path = 0;
            if ((path = (next_region == msg->dest_region)))
			{
				ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
				e = tw_event_new(msg->dest_airport, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING_REQ;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
			}
			else
			{
				ts = bs_rand_exponential2(s->rn, MEAN_REQ, lp);
				
				e = tw_event_new(next_region, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = TRANSIT_REQ;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
			}
			
			tw_event_send(e);
			
			__store__(path,lp);
			
			break;
		}
			
		case TRANSIT_DELAY:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
        case LANDING_REQ:
        {
			int weather = bs_rand_integer2(s->rn, 0, 3, lp);
			
			int path = 0;
			
            if ((path = (s->runway_in_use < s->max_runway))) 
            {
				__store__(s->runway_in_use, lp);
				__store__(s->landing_req_accepted, lp);
				
                s->runway_in_use++;
                s->landing_req_accepted++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
            }
            else
            {
				__store__(s->landing_req_rejected, lp);
				
				s->landing_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
            }
			
			tw_event_send(e);
			
			__store__(path,lp);
			
            break;
        }
			
		case LANDING_DELAY:
		{	
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = LANDING_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case LANDING:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_TAXI, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAXI_IN;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}	
			
		case TAXI_IN:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_ARRIVAL, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case ARRIVAL:
		{
			__store__(s->runway_in_use, lp);
			s->runway_in_use--;
			
			int dest = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
			
			ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
			
            e = tw_event_new(dest, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
			
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
			//cout<<"RE-DEP_REQ"<<endl;
			
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path) {
				
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);
				
				__restore__(s->dep_req_accepted, lp);
				__restore__(s->runway_in_use, lp);
				
			}
			else {
				
				bs_rand_rvs(s->rn, lp);
				__restore__(s->dep_req_rejected, lp);
			}
			
			bs_rand_rvs(s->rn, lp);
			
            break;
        }
            
        case DEP_DELAY:
        {
			//cout<<"RE-DEP_DELAY"<<endl;
			
            bs_rand_rvs(s->rn, lp);
            
            break;
        }
			
        case TAXI_OUT:
        {
			//cout<<"RE-TAXI_OUT"<<endl;
			
            bs_rand_rvs(s->rn, lp);
            
            break;
        }
            
        case TAKE_OFF:
        {
			//cout<<"RE-TAKE_OFF"<<endl;
			
			bs_rand_rvs(s->rn, lp);
			__restore__(s->runway_in_use, lp);
			
            break;
        }  
            
        case TRANSIT_REQ:
        {
			//cout<<"RE-TRANSIT_REQ"<<endl;
			
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path) 
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->transit_req_accepted, lp);
				__restore__(s->airplane_in_region, lp);
			}
			else 
			{
				bs_rand_rvs(s->rn, lp);
				
				__restore__(s->transit_req_rejected, lp);
			}
			
            break;
        }
            
		case ON_THE_AIR:
		{
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path)
			{
				bs_rand_rvs(s->rn, lp);
				
			}
			else
			{
				bs_rand_rvs(s->rn, lp);
				
			}
			
			__restore__(s->airplane_in_region, lp);
			bs_rand_rvs(s->rn, lp);
			
            break;
		}
			
        case TRANSIT_DELAY:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case LANDING_REQ:
		{
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path)
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->landing_req_accepted, lp);
				__restore__(s->runway_in_use, lp);
				
			}
			else
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->landing_req_rejected, lp);
				
			}
			
			bs_rand_rvs(s->rn, lp);
			
            break;
		}
			
		case LANDING_DELAY:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case LANDING:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case TAXI_IN:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        }
			
		case ARRIVAL:
        {			
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
			
			__restore__(s->runway_in_use, lp);
			
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
    
    total_landing_req_accepted += s->landing_req_accepted;
    total_landing_req_rejected += s->landing_req_rejected;
    
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
/*
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
 */

const tw_optdef app_opt [] =
{
	TWOPT_GROUP("Airport Model"),
	//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
	TWOPT_UINT("nplanes", planes_per_airport, "initial # of planes per airport(events)"),
	TWOPT_STIME("mean", mean_flight_time, "mean flight time for planes"),
	TWOPT_UINT("memory", opt_mem, "optimistic memory"),
	TWOPT_END()
};

static unsigned int nkp_per_pe = 16;

void air_traffic_mapping()
{
    cout<<"air_traffic_mapping"<<endl;
    
    tw_pe	*pe;
    
	int	 nlp_per_kp;
	int	 lpid;
	int	 kpid;
	int	 i;
	int	 j;
    
    nlp_per_kp = ceil((double) g_tw_nlp / (double) g_tw_nkp);
    
    //    printf("nlp_per_kp : %d (g_tw_nlp, %d / g_tw_nkp, %d) \n", nlp_per_kp, g_tw_nlp, g_tw_nkp);
    
	if(!nlp_per_kp)
		tw_error(TW_LOC, "Not enough KPs defined: %d", g_tw_nkp);
    
	g_tw_lp_offset = g_tw_mynode * g_tw_nlp;
    
    //    printf("g_tw_lp_offset = %d (g_tw_mynode,%d * g_tw_nlp, %d)\n", g_tw_lp_offset, g_tw_mynode, g_tw_nlp);
    
    kpid = 0;
    lpid = 0;
    
    if(g_tw_mynode == 0)
    {
        printf("\tPE %d\n", tw_getpe(0)->id);
        
        for(i = 0; i < nkp_per_pe; i++, kpid++)
        {
            tw_kp_onpe(kpid, tw_getpe(0));
            
            printf("\t\tKP %d", kpid);
            
            for(j = 0; j < nlp_per_kp && lpid < g_tw_nlp; j++, lpid++)
            {
                
                tw_lp_onpe(lpid, tw_getpe(0), g_tw_lp_offset+lpid);
                tw_lp_onkp(g_tw_lp[lpid], g_tw_kp[kpid]); 
                
                if(0 == j % 20)
                    printf("\n\t\t\t");
                printf("%lld ", lpid+g_tw_lp_offset);
            }
            
            printf("\n");
        }
    }
    else
    {
        printf("\tPE %d\n", tw_getpe(0)->id);
        
        for(i = 0; i < nkp_per_pe; i++, kpid++)
        {
            tw_kp_onpe(kpid, tw_getpe(0));
            
            printf("\t\tKP %d", kpid);
            
            for(j = 0; j < nlp_per_kp && lpid < g_tw_nlp; j++, lpid++)
            {
                
                tw_lp_onpe(lpid, tw_getpe(0), g_tw_lp_offset+lpid);
                tw_lp_onkp(g_tw_lp[lpid], g_tw_kp[kpid]); 
                
                if(0 == j % 20)
                    printf("\n\t\t\t");
                printf("%lld ", lpid+g_tw_lp_offset);
            }
            
            printf("\n");
        }
    }
    
    
    //    for(kpid = 0, lpid = 0, pe = NULL; (pe = tw_pe_next(pe)); )
    //	{
    //		printf("\tPE %d\n", pe->id);
    //        
    //		for(i = 0; i < nkp_per_pe; i++, kpid++)
    //		{
    //			tw_kp_onpe(kpid, pe);
    //            
    //			printf("\t\tKP %d", kpid);
    //            
    //			for(j = 0; j < nlp_per_kp && lpid < g_tw_nlp; j++, lpid++)
    //			{
    //                
    //                tw_lp_onpe(lpid, pe, g_tw_lp_offset+lpid);
    //                tw_lp_onkp(g_tw_lp[lpid], g_tw_kp[kpid]); 
    //                
    //                if(0 == j % 20)
    //                    printf("\n\t\t\t");
    //                printf("%lld ", lpid+g_tw_lp_offset);
    //			}
    //            
    //			printf("\n");
    //		}
    //	}
}

int
main(int argc, char **argv, char **env)
{
	cout << "####"<<endl;
	cout << "ROSE" << endl;;
	cout << "####"<<endl;
	
	int i;
    
	tw_opt_add(app_opt);
	tw_init(&argc, &argv);
    
	nlp_per_pe /= (tw_nnodes() * g_tw_npe);
    
    g_tw_mapping = CUSTOM;
    g_tw_custom_initial_mapping = &air_traffic_mapping;
    
    
	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(air_traffic_message), 0);
    
    
    graph = new Graph(20);
    graph->create_graph(GRAPH_CSV_FILE_PATH);
	//graph->print_adjmatrix();
    
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
        
        cout<<"\tTotal Arrival Req Accepted : "<<total_landing_req_accepted<<endl;
        cout<<"\tTotal Arrival Req Rejected : "<<total_landing_req_rejected<<endl;
        
        
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
    else if(airport >=54 && airport <= 73)
    {
        return 3;
    }
    else if(airport >=74 && airport <= 90)
    {
        return 4;
    }
    else if(airport >=91 && airport <= 124)
    {
        return 6;
    }    
    else if(airport >=125 && airport <= 143)
    {
        return 2;
    }
    else if(airport >=144 && airport <= 148)
    {
        return 5;
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
    else if(airport >=215 && airport <= 231)
    {
        return 12;
    }
    else if(airport >=232 && airport <= 256)
    {
        return 15;
    }    
    else if(airport >=257 && airport <= 264)
    {
        return 16;
    }    
    else if(airport >=265 && airport <= 280)
    {
        return 17;
    }    
    else if(airport >=281 && airport <= 292)
    {
        return 11;
    }    
    else if(airport >=293 && airport <= 303)
    {
        return 13;
    }    
    else if(airport >=304 && airport <= 318)
    {
        return 14;
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