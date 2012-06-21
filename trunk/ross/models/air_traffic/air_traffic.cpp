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
	s->incoming_queue = new priority_queue<Aircraft, vector<Aircraft>, less<Aircraft> >();
	assert(	s->incoming_queue->empty());

	s->aircraft_counter = new vector<int>(656);
	s->max_queue_size_airport = 0;
	s->max_queue_size_region = 0;

	s->delay_airport_dep = 0;
	s->delay_airport_land = 0;

	s->cdelay_airport_dep = 0;
	s->cdelay_airport_dep = 0;

	s->delay_region = 0;
	s->cdelay_region = 0;

	if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
	{
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_SMALL_REGION);
			
		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;	
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_MEDIUM_REGION);
			
		}
		else {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_LARGE_REGION);
			
		}

		s->airplane_in_region = 0;

		s->transit_req_accepted = 0;
		s->transit_req_rejected = 0;
		s->transit_processed = 0;

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
				lp->gid == 39  ||
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

		s->landing_processed = 0;
		s->landing_req_accepted=0;
		s->landing_req_rejected=0;

		s->dep_processed=0;
		s->dep_req_accepted = 0;
		s->dep_req_rejected=0;


		for(i = 0; i < planes_per_airport; i++)
		{
			int event_send_to = lp->gid;
			ts = bs_rand_exponential(s->rn, MEAN_DEQ);

			Aircraft aircraft;
			double dep_time = bs_rand_exponential(s->rn, MEAN_DEQ);
			dep_time += tw_now(lp);

			int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
			int dest_region = get_region(dest_airport);
			int max_speed = bs_rand_integer(s->rn, MIN_AIRCRAFT_SPEED, MAX_AIRCRAFT_SPEED);
			int src_region = get_region(lp->gid);

			aircraft.m_src_airport = lp->gid;
			aircraft.m_src_region = src_region;
			aircraft.m_process_time = dep_time;
			aircraft.m_dest_airport = dest_airport;
			aircraft.m_dest_region = dest_region;
			aircraft.m_max_speed = max_speed;
			aircraft.m_speed = max_speed;

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

				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);
				s->incoming_queue->push(aircraft);

				if(s->incoming_queue->size() > s->max_queue_size_airport)
					s->max_queue_size_airport = s->incoming_queue->size();

				if (s->runway_in_use < s->max_runway) 
				{
					s->runway_in_use++;
					s->dep_req_accepted++;
					s->dep_processed++;

					Aircraft aircraft = s->incoming_queue->top();
					s->incoming_queue->pop();

					s->delay_airport_dep += tw_now(lp) - aircraft.m_clock;
					s->cdelay_airport_dep += aircraft.m_cdelay;

					aircraft.m_clock = 0;
					aircraft.m_cdelay = 0;
					aircraft.m_delay = 0;

					int to = lp->gid;
					ts = bs_rand_exponential(s->rn, MEAN_DEQ);

					e = tw_event_new(to, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TAXI_OUT;
					m->aircraft = aircraft;

					tw_event_send(e);
				}
				else
				{
					s->dep_req_rejected++;
				}

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();
						temp.m_cdelay++;
						//cout<<temp.m_cdelay<<endl;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp =temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);

				}

				break;
			}

		case DEP:
			{
				if (s->runway_in_use < s->max_runway)
				{
					if(s->incoming_queue->size() > 0)
					{
						s->runway_in_use++;
						s->dep_processed++;

						Aircraft aircraft = s->incoming_queue->top();
						s->incoming_queue->pop();

						s->delay_airport_dep += tw_now(lp) - aircraft.m_clock;
						s->cdelay_airport_dep += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;

						int to = lp->gid;
						ts = bs_rand_exponential(s->rn, MEAN_DEQ);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = TAXI_OUT;
						m->aircraft = aircraft;

						tw_event_send(e);

					}
				}

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();
						temp.m_cdelay++;
						//cout<<temp.m_cdelay<<endl;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp =temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);

				}

				break;
			}

		case TAXI_OUT:
			{
				//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
				(*(s->aircraft_counter))[msg->aircraft.m_id]++;

				int to = lp->gid;				
				ts = bs_rand_exponential(s->rn, MEAN_TAXI);

				e = tw_event_new(to, ts, lp);

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
				int next_region = 0;
				std::deque<int> path = graph->get_shortest_path(src_region, msg->aircraft.m_dest_region);

				if (path.size() != 1) 
				{
					path.pop_front();
					next_region = path.front();
				}
				else 
				{
					next_region = path.front();;
					assert(next_region == msg->aircraft.m_dest_region);
				}

				float distance = graph->adjMatrix[src_region][next_region];
				if(distance < 0) distance = 0;

				int to = next_region;
				ts = bs_rand_exponential(s->rn, MEAN_TAKE_OFF);

				e = tw_event_new(to, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = TRANSIT_REQ;
				m->aircraft = msg->aircraft;
				m->aircraft.m_remaining_dist = distance;
				m->aircraft.m_process_time = distance;
				tw_event_send(e);


				/*
				   Schedule Another Event 
				   Without this event, the simulation will be terminated soon because initial events fed to the engine
				   run out pretty quickly due to the limited resources
				 */

				int to2 = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_DEQ);

				e = tw_event_new(to2, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = DEP;
				m->aircraft = msg->aircraft;

				tw_event_send(e);

				break;
			}
			/*
			   Region Controller Handles This Event
			 */
		case TRANSIT_REQ:
			{
				assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				
				Aircraft aircraft = msg->aircraft;
				
				aircraft.m_clock = tw_now(lp);
				
				s->incoming_queue->push(aircraft);

				if(s->incoming_queue->size() > s->max_queue_size_region )
					s->max_queue_size_region = s->incoming_queue->size();

//				if (s->airplane_in_region < s->max_capacity)
				if(s->controller->m_current_capacity < s->controller->m_max_capacity)
				{

					Aircraft aircraft = s->incoming_queue->top();

					if(aircraft.m_remaining_dist <= 0)
					{
						s->controller->m_current_capacity++;
						

						s->airplane_in_region++;
						s->transit_req_accepted++;
						s->transit_processed++;

						s->incoming_queue->pop();

						s->delay_region += tw_now(lp) - aircraft.m_clock;
						s->cdelay_region += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;

						int to = lp->gid;
						ts = bs_rand_exponential(s->rn, MEAN_FLIGHT);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = ON_THE_AIR;
						m->aircraft = aircraft;
						m->msg_from = lp->gid;

						tw_event_send(e);
					}
				}
				else
				{
					
					assert(s->incoming_queue->size() > 0);
					s->transit_req_rejected++;
				}
				//update remaining distance, process time, delays

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();

						temp.m_process_time -= temp.m_speed;
						temp.m_remaining_dist -= temp.m_speed;
						//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;

						temp.m_cdelay++;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp =temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					//assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);
				}

				break;
			}

		case TRANSIT:
			{
				//assert(false);

				if(s->controller->m_current_capacity < s->controller->m_max_capacity)				
				//if (s->airplane_in_region < s->max_capacity) 
				{
					if(s->incoming_queue->size() > 0)
					{
						s->transit_processed++;

						Aircraft aircraft = s->incoming_queue->top();

						if(aircraft.m_remaining_dist <= 0)
						{
							s->controller->m_current_capacity++;
							
							s->incoming_queue->pop();

							s->airplane_in_region++;

							s->delay_region += tw_now(lp) - aircraft.m_clock;
							s->cdelay_region += aircraft.m_cdelay;

							aircraft.m_clock = 0;
							aircraft.m_cdelay = 0;
							aircraft.m_delay = 0;

							int to = lp->gid;
							ts = bs_rand_exponential(s->rn, MEAN_FLIGHT);

							e = tw_event_new(to, ts, lp);

							m = (air_traffic_message*)tw_event_data(e);
							m->type = ON_THE_AIR;
							m->aircraft = aircraft;
							m->msg_from = lp->gid;

							tw_event_send(e);
						}
					}
				}

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();

						temp.m_process_time -= temp.m_speed;						
						temp.m_remaining_dist -= temp.m_speed;
						//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;

						temp.m_cdelay++;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp = temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					//assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);
				}					

				break;
			}

			/*
			   Region Controller Handles This Event
			 */

		case ON_THE_AIR:
			{
				assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				s->airplane_in_region--;

				s->controller->m_current_capacity--;
				

				int src_region = lp->gid;
				int next_region = 0;
				std::deque<int> path = graph->get_shortest_path(src_region, msg->aircraft.m_dest_region);

				if (path.size() != 1) 
				{
					path.pop_front();
					next_region = path.front();
				}
				else 
				{
					next_region = path.front();;
					assert(next_region == msg->aircraft.m_dest_region);
				}

				if (next_region == msg->aircraft.m_dest_region)
				{
					int to = msg->aircraft.m_dest_airport;
					ts = bs_rand_exponential(s->rn, MEAN_LAND);

					e = tw_event_new(to, ts, lp);

					double land_time = bs_rand_exponential(s->rn, MEAN_LAND);
					land_time += tw_now(lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = LANDING_REQ;
					m->aircraft = msg->aircraft;
					m->aircraft.m_process_time = land_time;
					m->msg_from = lp->gid;

					tw_event_send(e);

					/*
					   Schedule Transit Event
					 */


					int to2 = lp->gid;
					ts = bs_rand_exponential(s->rn, MEAN_FLIGHT);

					e = tw_event_new(to2, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TRANSIT;
					m->aircraft = msg->aircraft;

					tw_event_send(e);


				}
				else
				{
					int to = next_region;
					ts = bs_rand_exponential(s->rn, MEAN_REQ);

					e = tw_event_new(to, ts, lp);

					float distance = graph->adjMatrix[src_region][next_region];
					if(distance < 0) distance = 0;

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TRANSIT_REQ;
					m->aircraft = msg->aircraft;
					m->aircraft.m_process_time = distance;
					m->aircraft.m_remaining_dist = distance;
					m->msg_from = lp->gid;

					tw_event_send(e);

				}

				break;
			}

		case LANDING_REQ:
			{

				assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);

				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);
				s->incoming_queue->push(aircraft);

				if(s->incoming_queue->size() > s->max_queue_size_airport)
					s->max_queue_size_airport = s->incoming_queue->size();

				if (s->runway_in_use < s->max_runway) 
				{
					s->runway_in_use++;
					s->landing_req_accepted++;
					s->landing_processed++;

					Aircraft aircraft = s->incoming_queue->top();
					s->incoming_queue->pop();

					s->delay_airport_land += tw_now(lp) - aircraft.m_clock;
					s->cdelay_airport_land += aircraft.m_cdelay;

					aircraft.m_clock = 0;
					aircraft.m_cdelay = 0;
					aircraft.m_delay = 0;

					int to = lp->gid;
					ts = bs_rand_exponential(s->rn, MEAN_DEQ);

					e = tw_event_new(to, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TAXI_IN;
					m->aircraft = aircraft;

					tw_event_send(e);
				}
				else
				{
					s->landing_req_rejected++;
				}

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();
						temp.m_cdelay++;
						//cout<<temp.m_cdelay<<endl;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp =temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);

				}

				break;
			}

		case LANDING:
			{
				if (s->runway_in_use < s->max_runway)
				{
					if( s->incoming_queue->size() > 0)
					{
						s->runway_in_use++;
						s->landing_processed++;

						Aircraft aircraft = s->incoming_queue->top();
						s->incoming_queue->pop();

						int to = lp->gid;
						ts = bs_rand_exponential(s->rn, MEAN_LAND);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = TAXI_IN;
						m->aircraft = aircraft;
						m->msg_from = lp->gid;
						tw_event_send(e);				
					}
				}

				if(s->incoming_queue->size() > 0)
				{
					priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();

					Aircraft old_top = s->incoming_queue->top();
					int old_size = s->incoming_queue->size();

					while(!s->incoming_queue->empty())
					{
						Aircraft temp = s->incoming_queue->top();
						s->incoming_queue->pop();
						temp.m_cdelay++;
						//cout<<temp.m_cdelay<<endl;
						temp_q->push(temp);
					}

					while(!temp_q->empty())
					{
						Aircraft temp =temp_q->top();
						temp_q->pop();

						s->incoming_queue->push(temp);
					}

					Aircraft new_top = s->incoming_queue->top();
					int new_size = s->incoming_queue->size();

					delete temp_q;

					assert(old_top.m_id == new_top.m_id);
					assert(old_size == new_size);

				}

				break;
			}	

		case TAXI_IN:
			{
				(*(s->aircraft_counter))[msg->aircraft.m_id]++;
				
				int to = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_ARRIVAL);

				e = tw_event_new(to, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = ARRIVAL;
				m->aircraft = msg->aircraft;
				m->msg_from = lp->gid;

				tw_event_send(e);

				break;
			}

		case ARRIVAL:
			{
				s->runway_in_use--;

				int to = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_DEQ);

				Aircraft aircraft = msg->aircraft;

				double dep_time = bs_rand_exponential(s->rn, MEAN_DEQ);
				dep_time += tw_now(lp);

				int dest_airport = bs_rand_integer(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1);
				int dest_region = get_region(dest_airport);
				int max_speed = bs_rand_integer(s->rn, MIN_AIRCRAFT_SPEED, MAX_AIRCRAFT_SPEED);
				int src_region = get_region(lp->gid);

				aircraft.m_src_airport = lp->gid;
				aircraft.m_src_region = src_region;
				aircraft.m_process_time = dep_time;
				aircraft.m_dest_airport = dest_airport;
				aircraft.m_dest_region = dest_region;
				aircraft.m_max_speed = max_speed;
				aircraft.m_speed = max_speed;

				e = tw_event_new(to, ts, lp);            
				m = (air_traffic_message*)tw_event_data(e);
				m->type = DEP_REQ;
				m->aircraft = aircraft;

				tw_event_send(e);

				/*
				   Schedule another event
				 */
				int to2 = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_FLIGHT);

				e = tw_event_new(to2, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING;
				m->aircraft = msg->aircraft;

				tw_event_send(e);

				break;
			}
	}
}



void final(airport_state * s, tw_lp * lp)

{
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
	total_transit_req_accepted += s->transit_req_accepted;
	total_transit_req_rejected += s->transit_req_rejected;
	total_transit_processed += s->transit_processed;

	total_dep_processed += s->dep_processed;
	total_dep_req_accepted += s->dep_req_accepted;
	total_dep_req_rejected += s->dep_req_rejected;

	total_landing_processed += s->landing_processed;
	total_landing_req_accepted += s->landing_req_accepted;
	total_landing_req_rejected += s->landing_req_rejected;

	max_queue_size_airport = s->max_queue_size_airport > max_queue_size_airport ? s->max_queue_size_airport : max_queue_size_airport;
	max_queue_size_region = s->max_queue_size_region > max_queue_size_region ? s->max_queue_size_region : max_queue_size_region;

	total_cdelay_airport_dep += s->cdelay_airport_dep;
	total_delay_airport_dep += s->delay_airport_dep;

	total_cdelay_airport_land += s->cdelay_airport_land;
	total_delay_airport_land += s->delay_airport_land;

	total_cdelay_region += s->cdelay_region;
	total_delay_region += s->delay_region;

	//cout<<lp->gid<<","<<total_cdelay<<endl;
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

		std::cout<<"\tTotal Transit Req Accepted : "<<total_transit_req_accepted<<std::endl;
		std::cout<<"\tTotal Transit Req Rejected : "<<total_transit_req_rejected<<std::endl;
		std::cout<<"\tTotal Transit Processed : "<<total_transit_processed<<std::endl;
		std::cout<<""<<endl;


		std::cout<<"\tTotal Departure Req Accepted : "<<total_dep_req_accepted<<std::endl;
		std::cout<<"\tTotal Departure Req Rejected : "<<total_dep_req_rejected<<std::endl;
		std::cout<<"\tTotal Departure Processed : "<<total_dep_processed<<std::endl;
		std::cout<<""<<endl;

		std::cout<<"\tTotal Landing Req Accepted : "<<total_landing_req_accepted<<std::endl;
		std::cout<<"\tTotal Landing Req Rejected : "<<total_landing_req_rejected<<std::endl;
		std::cout<<"\tTotal Landing Processed : "<<total_landing_processed<<std::endl;
		std::cout<<""<<endl;

		std::cout<<"\tAvg Airport Dep cdelay : "<<(double)total_cdelay_airport_dep/total_dep_processed<<std::endl;		
		std::cout<<"\tAvg Airport Dep delay : "<<(double)total_delay_airport_dep/total_dep_processed<<std::endl;		
		std::cout<<""<<endl;

		std::cout<<"\tAvg Airport Land cdelay : "<<(double)total_cdelay_airport_land/total_landing_processed<<std::endl;		
		std::cout<<"\tAvg Airport Land delay : "<<(double)total_delay_airport_land/total_landing_processed<<std::endl;		
		std::cout<<""<<endl;

		std::cout<<"\tAvg Region cdelay : "<<(double)total_cdelay_region/total_transit_processed<<std::endl;		
		std::cout<<"\tAvg Region delay : "<<(double)total_delay_region/total_transit_processed<<std::endl;		
		std::cout<<""<<endl;

		std::cout<<"\tMax Airport Queue Size : "<<max_queue_size_airport<<std::endl;
		std::cout<<"\tMax Region Queue Size : "<<max_queue_size_region<<std::endl;


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
