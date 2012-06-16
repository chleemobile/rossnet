#include "air_traffic.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>

#define P_RUN 1
#define PADDING_LOOP 1
#define COUNT_EVENT 0


/*
   Air_Traffic.cpp
   Air Traffic Simulator
   20120406
   Chayong Lee

   2008/2/16
   Modified for ROSS 4.0
   David Bauer
 */

int get_region(int airport);
int mapping_to_local_index(int lpid);
int increase_counter(int lipd, int event_type);
int decrease_counter(int lipd, int event_type);
void write_map();


tw_peid mapping_to_pe(tw_lpid gid)
{    

	if(tw_nnodes() == 1)
	{
		return gid/nlp_per_pe;
	}
	else if(tw_nnodes() == 2)
	{
		if(gid < 10 || (gid >= 20 && gid <= 183))
		{
			//cout<<gid<<" return 0"<<endl;
			return 0;
		}
		else
		{
			//cout<<gid<<" return 1"<<endl;
			return 1;
		}

	}
	else if(tw_nnodes() == 4)
	{
		if(gid == 0|| gid == 1 || gid == 3 || gid == 4 || gid == 6 || (gid >= 20 && gid <= 101))
		{
			return 0;
		}
		else if(gid == 2|| gid == 5 || gid == 7 || gid == 8 || gid == 9 || (gid >= 102 && gid <= 183))
		{
			return 1;
		}
		else if(gid == 10|| gid == 12 || gid == 15 || gid == 16 || gid == 17 || (gid >= 184 && gid <= 265))
		{
			return 2;
		}
		else 
		{
			return 3;
		}
	}
	else 
	{
		std::cout << "support upto 4 cores"<<std::endl;
		assert(false);
	}

}

void init(airport_state * s, tw_lp * lp)
{
	static int init_seed = lp->gid;
	BSStack* stack = new BSStack();
	lp->stack_pointer = stack;

	int i;
	tw_event *e;
	air_traffic_message *m;
	tw_stime ts;

	s->rn=lp->gid;
	s->q = new priority_queue<Aircraft, vector<Aircraft>, less<Aircraft> >();

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

		s->delay = 0;
		s->cdelay = 0;

		for(i = 0; i < planes_per_airport; i++)
		{
			int event_send_to = lp->gid;
			ts = bs_rand_exponential(s->rn, MEAN_DEQ);

			Aircraft *aircraft = new Aircraft();

			double dep_time = bs_rand_exponential(s->rn, MEAN_DEQ);
			dep_time += tw_now(lp);

			int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
			int dest_region = get_region(dest_airport);
			int max_speed = bs_rand_integer(s->rn, MIN_AIRCRAFT_SPEED, MAX_AIRCRAFT_SPEED);

			aircraft->m_dep_time = dep_time;
			aircraft->m_dest_airport = dest_airport;
			aircraft->m_dest_region = get_region(dest_airport);
			aircraft->m_max_speed = max_speed;
			aircraft->m_speed = max_speed;

			e = tw_event_new(event_send_to, ts, lp);            
			m = (air_traffic_message*)tw_event_data(e);
			m->type = DEP_REQ;
			m->aircraft = aircraft;

			tw_event_send(e);
		}

	}
}


void event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
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

					ts = bs_rand_exponential(s->rn, MEAN_DEQ);
					ts += weather;

					e = tw_event_new(lp->gid, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TAXI_OUT;
					m->aircraft = msg->aircraft;

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

					//tw_event_send(e);

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
				
				assert(msg->aircraft->m_dest_region < NUMBER_OF_REGION_CONTROLLER);

				ts = bs_rand_exponential(s->rn, MEAN_TAXI);

				e = tw_event_new(lp->gid, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = TAKE_OFF;
				m->aircraft = msg->aircraft;

				tw_event_send(e);

				break;
			}

		case TAKE_OFF:
			{            
				
				s->runway_in_use--;

				int src_region = get_region(lp->gid);
				int next_region =-1;
				std::deque<int> p = graph->get_shortest_path(src_region, msg->aircraft->m_dest_region);
				

				if (p.size() != 1) 
				{
					p.pop_front();
					next_region = p.front();
				}
				else 
				{
					next_region = msg->aircraft->m_dest_region;
				}

				ts = bs_rand_exponential(s->rn, MEAN_TAKE_OFF);

				e = tw_event_new(next_region, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = TRANSIT_REQ;
				m->aircraft = msg->aircraft;

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
					m->aircraft = msg->aircraft;
					m->msg_from = lp->gid;

					tw_event_send(e);

				}
				else
				{
					s->transit_req_rejected++;

					ts = bs_rand_exponential(s->rn, MEAN_DELAY);

					e = tw_event_new(lp->gid, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TRANSIT_DELAY;
					m->msg_from = lp->gid;

				}


				break;
			}

		case ON_THE_AIR:
			{

				assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				int next_region = bs_rand_integer(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1);

				std::deque<int> p = graph->get_shortest_path(lp->gid, msg->aircraft->m_dest_region);

				if (p.size() != 1) 
				{
					p.pop_front();
					next_region = p.front();
				}
				else 
				{
					next_region = msg->aircraft->m_dest_region;
				}

				s->airplane_in_region--;

				if (next_region == msg->aircraft->m_dest_region)
				{
					ts = bs_rand_exponential(s->rn, MEAN_LAND);
					e = tw_event_new(msg->aircraft->m_dest_airport, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = LANDING_REQ;
					m->aircraft = msg->aircraft;
					m->msg_from = lp->gid;
				}
				else
				{
					ts = bs_rand_exponential(s->rn, MEAN_REQ);

					e = tw_event_new(next_region, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TRANSIT_REQ;
					m->aircraft = msg->aircraft;
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
				m->msg_from = lp->gid;

				//tw_event_send(e);

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
					m->aircraft = msg->aircraft;
					m->msg_from = lp->gid;
					tw_event_send(e);				

				}
				else
				{
					s->landing_req_rejected++;

					ts = bs_rand_exponential(s->rn, MEAN_DELAY);
					ts += weather;

					e = tw_event_new(lp->gid, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = LANDING_DELAY;


				}


				break;
			}

		case LANDING_DELAY:
			{	
				ts = bs_rand_exponential(s->rn, MEAN_DELAY);

				e = tw_event_new(lp->gid, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING_REQ;
				m->msg_from = lp->gid;


				break;
			}

		case LANDING:
			{
				ts = bs_rand_exponential(s->rn, MEAN_TAXI);

				e = tw_event_new(lp->gid, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = TAXI_IN;
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
				m->msg_from = lp->gid;

				tw_event_send(e);

				break;
			}

		case ARRIVAL:
			{
				s->runway_in_use--;

				int event_send_to = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_DEQ);

				delete msg->aircraft;
				Aircraft *aircraft = new Aircraft();

				double dep_time = bs_rand_exponential(s->rn, MEAN_DEQ);
				dep_time += tw_now(lp);

				int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
				int dest_region = get_region(dest_airport);
				int max_speed = bs_rand_integer(s->rn, MIN_AIRCRAFT_SPEED, MAX_AIRCRAFT_SPEED);

				aircraft->m_dep_time = dep_time;
				aircraft->m_dest_airport = dest_airport;
				aircraft->m_dest_region = get_region(dest_airport);
				aircraft->m_max_speed = max_speed;
				aircraft->m_speed = max_speed;

				e = tw_event_new(event_send_to, ts, lp);            
				m = (air_traffic_message*)tw_event_data(e);
				m->type = DEP_REQ;
				m->aircraft = aircraft;

				tw_event_send(e);

				break;
			}

	}
}




/*******************
  Parallel
 ******************/
/*
   void p_init(airport_state * s, tw_lp * lp)
   {
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

void event_handler_fw(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    	
	tw_stime ts;
	tw_event *e;
	air_traffic_message *m;

	if(COUNT_EVENT) increase_counter(lp->gid, msg->type);

	switch(msg->type)
	{
		//        *(int *)bf = (int)0;

		case DEP_REQ:
			{
				assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);

				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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

				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}


				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);

				e = tw_event_new(lp->gid, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = DEP_REQ;
				tw_event_send(e);

				break;
			}

		case TAXI_OUT:
			{
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}


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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

				__store__(s->runway_in_use, lp);
				s->runway_in_use--;

				int src_region = get_region(lp->gid);
				int next_region =-1;
				std::deque<int> p = graph->get_shortest_path(src_region, msg->dest_region);
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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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

					tw_event_send(e);

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

					tw_event_send(e);

				}


				__store__(path,lp);

				break;
			}

		case ON_THE_AIR:
			{
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

				assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				int next_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp);

				std::deque<int> p = graph->get_shortest_path(lp->gid, msg->dest_region);

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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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

					tw_event_send(e);

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

					tw_event_send(e);

				}


				__store__(path,lp);

				break;
			}

		case LANDING_DELAY:
			{	
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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
				if(PADDING_LOOP)
					for (unsigned volatile long var = 0; var < loop_size;var++){}

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


void event_handler_rc(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    	
	if(COUNT_EVENT) decrease_counter(lp->gid, msg->type);

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
*/

void final(airport_state * s, tw_lp * lp)

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
		(init_f) init,
		(event_f) event_handler,
		(revent_f) event_handler,
		(final_f) final,
		(map_f) mapping_to_pe,
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
	TWOPT_UINT("loopsize", loop_size, "padding loop size"),
	TWOPT_UINT("prun", p_run, "parallel run"),

	TWOPT_END()
};

tw_lp* mapping_to_lp(tw_lpid lpid) 
{
	int ret = lpid;

	if(tw_nnodes() == 2)
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20)
			{
				assert(lpid < 184);
				ret = ret - 10;
			}
		}
		else
		{
			if(lpid >= 10 && lpid <= 19)
				ret = ret + 164;
			ret = ret - 174;
		}

	}
	else if (tw_nnodes() == 4) 
	{
		if(g_tw_mynode == 0)
		{
			if (lpid == 3 || lpid == 4 ) 
				ret = ret - 1;
			if(lpid == 6)
				ret = ret - 2;
			if(lpid >= 20 && lpid <= 101)
				ret = ret - 15;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 85;
			if(lpid == 5)
				ret = ret + 83;
			if(lpid >= 7 && lpid <= 9)
				ret = ret + 82;
			if(lpid >= 102 && lpid <= 183)
				ret = ret - 10;

			ret = ret - 87;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 10)
				ret = ret + 164;
			if(lpid == 12)
				ret = ret + 163;          
			if(lpid >= 15 && lpid <= 17)
				ret = ret + 161;                
			if(lpid >= 184 && lpid <= 265)
				ret = ret - 5;

			ret = ret - 174;
		}
		else 
		{
			if(lpid == 11)
				ret = ret + 250;
			if(lpid == 13 || lpid == 14)
				ret = ret + 249;
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 246;

			ret = ret - 261;
		}
	}
	else 
	{
		std::cout << "Only support upto 4 cores "<<std::endl;;
		assert(false);
	}

	return g_tw_lp[ret];
}

int mapping_to_local_index(int lpid)
{
	//    return lpid;

	int ret = lpid;
	if(tw_nnodes() == 2)
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20)
			{
				assert(lpid < 184);
				ret = ret - 10;
			}
		}
		else
		{
			if(lpid >= 10 && lpid <= 19)
				ret = ret + 164;
			ret = ret - 174;
		}

	}
	else if (tw_nnodes() == 4) 
	{
		if(g_tw_mynode == 0)
		{
			if (lpid == 3 || lpid == 4 ) 
				ret = ret - 1;
			if(lpid == 6)
				ret = ret - 2;
			if(lpid >= 20 && lpid <= 101)
				ret = ret - 15;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 85;
			if(lpid == 5)
				ret = ret + 83;
			if(lpid >= 7 && lpid <= 9)
				ret = ret + 82;
			if(lpid >= 102 && lpid <= 183)
				ret = ret - 10;

			ret = ret - 87;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 10)
				ret = ret + 164;
			if(lpid == 12)
				ret = ret + 163;          
			if(lpid >= 15 && lpid <= 17)
				ret = ret + 161;                
			if(lpid >= 184 && lpid <= 265)
				ret = ret - 5;

			ret = ret - 174;
		}
		else 
		{
			if(lpid == 11)
				ret = ret + 250;
			if(lpid == 13 || lpid == 14)
				ret = ret + 249;
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 246;

			ret = ret - 261;
		}
	}
	else 
	{
		std::cout << "Only support upto 4 cores "<<std::endl;;
		assert(false);
	}

	//printf("%d -> %d \n", lpid, ret);

	return ret;
}

void air_traffic_mapping()
{
	int kpid = 0;
	int	 nlp_per_kp;
	nlp_per_kp = ceil((double) g_tw_nlp / (double) g_tw_nkp);

	std::cout <<"nlp_per_kp"<<nlp_per_kp<<std::endl;

	int local_lp_count=0;

	for (int lpid = 0; lpid < NUMBER_OF_LP; lpid++)
	{
		if( g_tw_mynode == mapping_to_pe(lpid) )
		{
			kpid = local_lp_count/nlp_per_kp;
			//cout<<local_lp_count<<",="<<kpid<<endl;
			local_lp_count++; // MUST COME AFTER!! DO NOT PRE-INCREMENT ELSE KPID is WRONG!!

			if( kpid >= g_tw_nkp )
				tw_error(TW_LOC, "Attempting to mapping a KPid (%llu) for Global LPid %llu that is beyond g_tw_nkp (%llu)\n",
						kpid, lpid, g_tw_nkp );

			tw_lp_onpe(mapping_to_local_index(lpid), g_tw_pe[0], lpid);
			//printf("tw_lp_onpe(%d, %d, %d)\n", mapping_to_local_index(lpid), g_tw_mynode, lpid);

			if( g_tw_kp[kpid] == NULL ){
				tw_kp_onpe(kpid, g_tw_pe[0]);
				//printf("tw_kp_onpe(%d, %d)\n", kpid, g_tw_mynode);
			}
			tw_lp_onkp(g_tw_lp[mapping_to_local_index(lpid)], g_tw_kp[kpid]);
			//printf("tw_kp_onpe(%d, %d)\n", mapping_to_local_index(lpid), kpid);

			tw_lp_settype( mapping_to_local_index(lpid), &airport_lps[0]);
		}
	}
}

int main(int argc, char **argv, char **env)
{

	//int loop_size = LOOP_SCALE * atoi(argv[argc-1].c_str());
	//std::string ssize = argv[argc-1];
	//int size = atoi(ssize.c_str());
	//loop_size = LOOP_SCALE * size;
	//std::cout<<"loop size : "<<loop_size<<std::endl;
	//argv[argc-1] = "";
	//argc = argc-1;

	int i;

	tw_opt_add(app_opt);

	tw_init(&argc, &argv);

	nlp_per_pe /= (tw_nnodes() * g_tw_npe);

	if (tw_nnodes() != 1) 
	{
		g_tw_mapping = CUSTOM;
		g_tw_custom_initial_mapping = &air_traffic_mapping;
		g_tw_custom_lp_global_to_local_map =&mapping_to_lp;
	}

	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(air_traffic_message), 0);


	graph = new Graph(20);
	graph->create_graph(GRAPH_CSV_FILE_PATH);
	//graph->print_adjmatrix();

	if (tw_nnodes() == 1) 
	{
		for(i = 0; i < g_tw_nlp; i++)
			tw_lp_settype(i, &airport_lps[0]);
	}

	//init map to count how many events are executed by each lp
	//key lpid, value map<event type, counter>

	if(COUNT_EVENT)
	{
		for (i=0; i<NUMBER_OF_LP; i++) 
		{
			counters.insert(make_pair(i, inner_map()));
			for(int j=0; i< ARRIVAL+1; i++)
			{
				counter_container c = {0,0};
				counters[i].insert(std::make_pair(j,c));
			}
		}

	}

	printf("loop size %d \n", loop_size);

	tw_run();

	if(tw_ismaster())
	{
		printf("\nAir Traffic Model Statistics:\n");

		printf("\t%-50s %11lld\n", "Number of LPs (Airport, Region Controller)", 
				nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
				planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());

		std::cout<<"\tTotal Transit Accepted : "<<total_transit_accepted<<std::endl;
		std::cout<<"\tTotal Transit Rejected : "<<total_transit_rejected<<std::endl;

		std::cout<<"\tTotal Departure Req Accepted : "<<total_dep_req_accepted<<std::endl;
		std::cout<<"\tTotal Departure Req Rejected : "<<total_dep_req_rejected<<std::endl;

		std::cout<<"\tTotal Arrival Req Accepted : "<<total_landing_req_accepted<<std::endl;
		std::cout<<"\tTotal Arrival Req Rejected : "<<total_landing_req_rejected<<std::endl;


	}

	tw_end();

	/********************
	  Optional Analyze Tool
	 *********************/

	//cout<<path_cal<<endl;

	if(COUNT_EVENT) write_map();

	if(0)
	{
		//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		std::cout<<memusage<<","<<std::endl;
	}	

	return 0;
}

int increase_counter(int lpid, int event_type)
{
	counters[lpid][event_type].total_event_count++;
	counters[lpid][event_type].net_event_count++;
}

int decrease_counter(int lpid, int event_type)
{
	counters[lpid][event_type].net_event_count--;
}

void write_map()
{
	FILE *fp;
	std::stringstream sstm;
	std::string name = "lpmap";
	int mynode = g_tw_mynode;

	sstm<<tw_nnodes()<<"_"<<name<<"_"<<mynode<<".txt";


	std::string file_name = sstm.str();

	std::ofstream file(file_name.c_str());

	if (file.is_open()) 
	{
		std::map<int, inner_map>::iterator it;
		std::map<int, counter_container>::iterator inner_it;

		for ( it=counters.begin() ; it != counters.end(); it++ ) 
		{
			for( inner_it=(*it).second.begin(); inner_it != (*it).second.end(); inner_it++)
			{
				int event_type = (*inner_it).first;
				std::string event_name;
				if (event_type == 0) {
					event_name = "DEP_REQ";
				}
				else if(event_type== 1){
					event_name = "DEP_DELAY";
				}
				else if(event_type== 2){
					event_name = "TAXI_OUT";
				}
				else if(event_type== 3){
					event_name = "TAKE_OFF";
				}				
				else if(event_type== 4){
					event_name = "TRANSIT_REQ";
				}
				else if(event_type== 5){
					event_name = "ON_THE_AIR";
				}
				else if(event_type== 6){
					event_name = "TRANSIT_DELAY";
				}				
				else if(event_type== 7){
					event_name = "LANDING_REQ";
				}
				else if(event_type== 8){
					event_name = "LANDING_DELAY";
				}
				else if(event_type== 9){
					event_name = "LANDING";
				}
				else if(event_type== 10){
					event_name = "TAXI_IN";
				}
				else {
					event_name = "ARRIVAL";
				}	
				std::string lp_name;

				if ((*it).first < NUMBER_OF_REGION_CONTROLLER) {
					lp_name = "RC";
				}
				else {
					lp_name = "Airport";
				}

				file<<mynode<<","<<lp_name<<","<<(*it).first<<","<<event_name<<","<<(*inner_it).second.total_event_count<<","<<(*inner_it).second.net_event_count<<"\n";
			}
		}
	}
	else
	{
		std::cout<<"write_map, couldn't open a file"<<std::endl;
	}

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
		std::cout<<"airport region failed"<<std::endl;
		assert(false);
	}

}
