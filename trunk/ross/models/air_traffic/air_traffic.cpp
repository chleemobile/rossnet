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
		if(gid < 10 || (gid >= 20 && gid <= 185))
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
		if(gid == 0|| gid == 1 || gid == 3 || gid == 4 || gid == 6 || (gid >= 20 && gid <= 102))
		{
			return 0;
		}
		else if(gid == 2|| gid == 5 || gid == 7 || gid == 8 || gid == 9 || (gid >= 103 && gid <= 185))
		{
			return 1;
		}
		else if(gid == 10|| gid == 12 || gid == 15 || gid == 16 || gid == 17 || (gid >= 186 && gid <= 268))
		{
			return 2;
		}
		else 
		{
			return 3;
		}
	}
	else if(tw_nnodes() == 8)
	{
		if(gid == 0|| gid == 1 || gid == 3 || (gid >= 20 && gid <= 60))
		{
			return 0;
		}
		else if(gid == 2|| gid == 5 ||  (gid >= 61 && gid <= 102))
		{
			return 1;
		}
		else if(gid == 4|| gid == 6 ||  (gid >= 103 && gid <= 144))
		{
			return 2;
		}
		else if(gid == 7|| gid == 8 || gid == 9 || (gid >= 145 && gid <= 185))
		{
			return 3;
		}
		else if(gid == 10 || gid == 12 || (gid >= 186 && gid <= 227))
		{
			return 4;
		}
		else if(gid == 11 || gid == 13 || gid == 14 || (gid >= 228 && gid <= 268))
		{
			return 5;
		}
		else if(gid == 15 || gid == 16 || gid == 17 || (gid >= 269 && gid <= 309))
		{
			return 6;
		}
		else if(gid == 18 || gid == 19 || (gid >= 310 && gid <= 351))
		{
			return 7;
		}

	}
	else if(tw_nnodes() == 16)
	{
		if(gid == 0|| gid == 1 || (gid >= 20 && gid <= 39))
		{
			return 0;
		}
		else if(gid == 2|| gid == 5 ||  (gid >= 40 && gid <= 59))
		{
			return 1;
		}
		else if(gid == 3|| (gid >= 60 && gid <= 80))
		{
			return 2;
		}
		else if(gid == 4||  (gid >= 81 && gid <= 101))
		{
			return 3;
		}
		else if(gid == 6 || (gid >= 102 && gid <= 122))
		{
			return 4;
		}
		else if(gid == 7|| (gid >= 123 && gid <= 143))
		{
			return 5;
		}
		else if(gid == 8 || (gid >= 144 && gid <= 164))
		{
			return 6;
		}
		else if(gid == 9 || (gid >= 165 && gid <= 185))
		{
			return 7;
		}
		if(gid == 10|| (gid >= 186 && gid <= 206))
		{
			return 8;
		}
		else if(gid == 11|| (gid >= 207 && gid <= 227))
		{
			return 9;
		}
		else if(gid == 12|| (gid >= 228 && gid <= 248))
		{
			return 10;
		}
		else if(gid == 13||  (gid >= 249 && gid <= 269))
		{
			return 11;
		}
		else if(gid == 14 || (gid >= 270 && gid <= 290))
		{
			return 12;
		}
		else if(gid == 15 || gid == 16 || (gid >= 291 && gid <= 310))
		{
			return 13;
		}
		else if(gid == 17 || (gid >= 311 && gid <= 331))
		{
			return 14;
		}
		else if(gid == 18 || gid == 19 || (gid >= 332 && gid <= 351))
		{
			return 15;
		}
	}
	else if(tw_nnodes() == 32)
	{
		if(gid == 0 || (gid >= 20 && gid <= 29))
		{
			return 0;
		}
		else if(gid == 1 ||  (gid >= 30 && gid <= 39))
		{
			return 1;
		}
		else if(gid == 2 || (gid >= 40 && gid <= 49))
		{
			return 2;
		}
		else if(gid == 3 ||  (gid >= 50 && gid <= 59))
		{
			return 3;
		}
		else if(gid == 4 || (gid >= 60 && gid <= 69))
		{
			return 4;
		}
		else if(gid == 5 || (gid >= 70 && gid <= 79))
		{
			return 5;
		}
		else if(gid == 6 || (gid >= 80 && gid <= 89))
		{
			return 6;
		}
		else if(gid == 7 || (gid >= 90 && gid <= 99))
		{
			return 7;
		}
		else if(gid == 8 || (gid >= 100 && gid <= 109))
		{
			return 8;
		}
		else if(gid == 9 || (gid >= 110 && gid <= 119))
		{
			return 9;
		}
		else if(gid == 10 || (gid >= 120 && gid <= 129))
		{
			return 10;
		}
		else if(gid == 11 ||  (gid >= 130 && gid <= 139))
		{
			return 11;
		}
		else if(gid == 12 || (gid >= 140 && gid <= 149))
		{
			return 12;
		}
		else if(gid == 13 || (gid >= 150 && gid <= 159))
		{
			return 13;
		}
		else if(gid == 14 || (gid >= 160 && gid <= 169))
		{
			return 14;
		}
		else if(gid == 15 || (gid >= 170 && gid <= 179))
		{
			return 15;
		}
		else if(gid == 16 || (gid >= 180 && gid <= 189))
		{
			return 16;
		}
		else if(gid == 17 || (gid >= 190 && gid <= 199))
		{
			return 17;
		}
		else if(gid == 18 || (gid >= 200 && gid <= 209))
		{
			return 18;
		}
		else if(gid == 19 || (gid >= 210 && gid <= 219))
		{
			return 19;
		}
		else if(gid >= 220 && gid <= 230)
		{
			return 20;
		}
		else if(gid >= 231 && gid <= 241)
		{
			return 21;
		}
		else if(gid >= 242 && gid <= 252)
		{
			return 22;
		}
		else if(gid >= 253 && gid <= 263)
		{
			return 23;
		}
		else if(gid >= 264 && gid <= 274)
		{
			return 24;
		}
		else if(gid >= 275 && gid <= 285)
		{
			return 25;
		}
		else if(gid >= 286 && gid <= 296)
		{
			return 26;
		}
		else if(gid >= 297 && gid <= 307)
		{
			return 27;
		}
		else if(gid >= 308 && gid <= 318)
		{
			return 28;
		}
		else if(gid >= 319 && gid <= 329)
		{
			return 29;
		}
		else if(gid >= 330 && gid <= 340)
		{
			return 30;
		}
		else if(gid >= 341 && gid <= 351)
		{
			return 31;
		}
	}	
	
	else 
	{
		std::cout << "support upto 32 cores"<<std::endl;
		assert(false);
	}

}

void init(airport_state * s, tw_lp * lp)
{
	//init additional data memeber of LP to support Backstroke
	static int init_seed = lp->gid;
	BSStack* stack = new BSStack();
	lp->stack_pointer = stack;

	//lp->vector_pointer = get_gvt_vector();
	//lp->bs_record_stack_position = bs_record_stack_position;
	lp->bs_fossil_collect = bs_fossil_collect;

	int i;
	tw_event *e;
	air_traffic_message *m;
	tw_stime ts;

	int num_aircraft = (NUMBER_OF_LP - NUMBER_OF_REGION_CONTROLLER) * NUMBER_OF_PLANES_PER_AIRPORT;

	int num_aircraft_per_core = num_aircraft / tw_nnodes();
	int aircraft_id_offset = num_aircraft_per_core * g_tw_mynode;
	
	if(tw_nnodes() == 32)
	{
		if (g_tw_mynode == 0) aircraft_id_offset = 0;
		else if(g_tw_mynode == 1) aircraft_id_offset = 20;
		else if(g_tw_mynode == 2) aircraft_id_offset = 40;
		else if(g_tw_mynode == 3) aircraft_id_offset = 60;
		else if(g_tw_mynode == 4) aircraft_id_offset = 80;
		else if(g_tw_mynode == 5) aircraft_id_offset = 100;
		else if(g_tw_mynode == 6) aircraft_id_offset = 120;
		else if(g_tw_mynode == 7) aircraft_id_offset = 140;
		else if(g_tw_mynode == 8) aircraft_id_offset = 160;
		else if(g_tw_mynode == 9) aircraft_id_offset = 180;
		else if(g_tw_mynode == 10) aircraft_id_offset = 200;
		else if(g_tw_mynode == 11) aircraft_id_offset = 220;
		else if(g_tw_mynode == 12) aircraft_id_offset = 240;
		else if(g_tw_mynode == 13) aircraft_id_offset = 260;
		else if(g_tw_mynode == 14) aircraft_id_offset = 280;
		else if(g_tw_mynode == 15) aircraft_id_offset = 300;
		else if(g_tw_mynode == 16) aircraft_id_offset = 320;
		else if(g_tw_mynode == 17) aircraft_id_offset = 340;
		else if(g_tw_mynode == 18) aircraft_id_offset = 360;
		else if(g_tw_mynode == 19) aircraft_id_offset = 380;
		else if(g_tw_mynode == 20) aircraft_id_offset = 400;
		else if(g_tw_mynode == 21) aircraft_id_offset = 422;
		else if(g_tw_mynode == 22) aircraft_id_offset = 444;
		else if(g_tw_mynode == 23) aircraft_id_offset = 466;
		else if(g_tw_mynode == 24) aircraft_id_offset = 488;
		else if(g_tw_mynode == 25) aircraft_id_offset = 510;
		else if(g_tw_mynode == 26) aircraft_id_offset = 532;
		else if(g_tw_mynode == 27) aircraft_id_offset = 554;
		else if(g_tw_mynode == 28) aircraft_id_offset = 576;
		else if(g_tw_mynode == 29) aircraft_id_offset = 598;
		else if(g_tw_mynode == 30) aircraft_id_offset = 620;
		else if(g_tw_mynode == 31) aircraft_id_offset = 642;
	}
	else if(tw_nnodes() == 16)
	{
		if (g_tw_mynode == 0) aircraft_id_offset = 0;
		else if(g_tw_mynode == 1) aircraft_id_offset = 40;
		else if(g_tw_mynode == 2) aircraft_id_offset = 80;
		else if(g_tw_mynode == 3) aircraft_id_offset = 122;
		else if(g_tw_mynode == 4) aircraft_id_offset = 164;
		else if(g_tw_mynode == 5) aircraft_id_offset = 206;
		else if(g_tw_mynode == 6) aircraft_id_offset = 248;
		else if(g_tw_mynode == 7) aircraft_id_offset = 290;
		else if(g_tw_mynode == 8) aircraft_id_offset = 332;
		else if(g_tw_mynode == 9) aircraft_id_offset = 374;
		else if(g_tw_mynode == 10) aircraft_id_offset = 416;
		else if(g_tw_mynode == 11) aircraft_id_offset = 458;
		else if(g_tw_mynode == 12) aircraft_id_offset = 500;
		else if(g_tw_mynode == 13) aircraft_id_offset = 542;
		else if(g_tw_mynode == 14) aircraft_id_offset = 582;
		else if(g_tw_mynode == 15) aircraft_id_offset = 624;
	}
	else if (tw_nnodes() ==8)
	{
		if (g_tw_mynode == 0) aircraft_id_offset = 0;
		else if(g_tw_mynode == 1) aircraft_id_offset = 82;
		else if(g_tw_mynode == 2) aircraft_id_offset = 166;
		else if(g_tw_mynode == 3) aircraft_id_offset = 250;
		else if(g_tw_mynode == 4) aircraft_id_offset = 332;
		else if(g_tw_mynode == 5) aircraft_id_offset = 416;
		else if(g_tw_mynode == 6) aircraft_id_offset = 498;
		else if(g_tw_mynode == 7) aircraft_id_offset = 580;

	}
	//cout<<aircraft_id_offset<<endl;
	
	s->rn=lp->gid;

	s->counter = new vector<int>(num_aircraft);

	s->delay_airport_dep = 0;
	s->delay_airport_land = 0;

	s->cdelay_airport_dep = 0;
	s->cdelay_airport_dep = 0;

	s->delay_region = 0;
	s->cdelay_region = 0;

	s->max_queue_size_airport = 0;
	s->max_queue_size_region = 0;

	s->transit_req_accepted = 0;
	s->transit_req_rejected = 0;
	s->transit_processed = 0;

	s->landing_processed = 0;
	s->landing_req_accepted=0;
	s->landing_req_rejected=0;

	s->dep_processed=0;
	s->dep_req_accepted = 0;
	s->dep_req_rejected=0;

	s->max_counter_aircraft_id =0;
	s->max_counter = 0;

	if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
	{
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_SMALL_REGION, num_aircraft);

		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_MEDIUM_REGION, num_aircraft);

		}
		else {
			s->controller = new RegionController(AIRCRAFT_CAPACITY_OF_LARGE_REGION, num_aircraft);

		}
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
			s->controller = new LocalTrafficController(NUMBER_OF_RUNWAY_LARGE_AIRPORT, num_aircraft);
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
			s->controller = new LocalTrafficController(NUMBER_OF_RUNWAY_MEDIUM_AIRPORT, num_aircraft);


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
			s->controller = new LocalTrafficController(NUMBER_OF_RUNWAY_SMALL_AIRPORT, num_aircraft);

		}
		else
		{
			s->controller = new LocalTrafficController(NUMBER_OF_RUNWAY_NH_AIRPORT, num_aircraft);

		}

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
			aircraft.m_id += aircraft_id_offset;

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

				//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);

				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);

				s->controller->add_aircraft(aircraft, lp);
				//s->controller->m_q.push(aircraft);

				if(s->controller->m_q.size() > s->max_queue_size_airport)
					s->max_queue_size_airport = s->controller->m_q.size();

				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					s->controller->handle_incoming(lp);

					s->dep_req_accepted++;
					s->dep_processed++;

					Aircraft aircraft = s->controller->get_aircraft(lp);
					s->controller->remove_aircraft(lp);
					//Aircraft aircraft = s->controller->m_q.top();
					//s->controller->m_q.pop();

					s->delay_airport_dep = s->delay_airport_dep + (tw_now(lp) - aircraft.m_clock);
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

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

		case DEP:
			{
				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					if(s->controller->m_q.size() > 0)
					{
						s->controller->handle_incoming(lp);

						s->dep_processed++;

						Aircraft aircraft = s->controller->get_aircraft(lp);
						s->controller->remove_aircraft(lp);
						//Aircraft aircraft = s->controller->m_q.top();
						//s->controller->m_q.pop();

						s->delay_airport_dep = s->delay_airport_dep + ( tw_now(lp) - aircraft.m_clock );
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

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

		case TAXI_OUT:
			{
				//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
				//s->controller->m_counter[msg->aircraft.m_id]++;

				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				 (*(s->counter))[msg->aircraft.m_id] = c;

				int i=0;
				while(i < num_aircraft)
				{
                    const vector<int>& counterRef = *(s->counter);
					int counter = counterRef.at(i);
					//int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}

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
				s->controller->handle_outgoing(lp);

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
					//assert(next_region == msg->aircraft.m_dest_region);
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
				//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
	
				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);

				s->controller->add_aircraft(aircraft, lp);
				//s->controller->m_q.push(aircraft);

				if(s->controller->m_q.size() > s->max_queue_size_region)
					s->max_queue_size_region = s->controller->m_q.size();

				if(s->controller->m_current_capacity < s->controller->m_max_capacity)
				{

					Aircraft aircraft = s->controller->get_aircraft(lp);
					//Aircraft aircraft = s->controller->m_q.top();

					if(aircraft.m_remaining_dist <= 0)
					{
						s->transit_req_accepted++;
						s->transit_processed++;

						s->controller->handle_incoming(lp);
						s->controller->remove_aircraft(lp);
						//s->controller->m_q.pop();

						s->delay_region = s->delay_region + (tw_now(lp) - aircraft.m_clock);
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
					s->transit_req_rejected++;
				}

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

			/*
			   Region Controller Handles This Event
			 */

		case TRANSIT:
			{
				if(s->controller->m_current_capacity < s->controller->m_max_capacity)
				{
					if(s->controller->m_q.size() > 0)
					{
						Aircraft aircraft = s->controller->get_aircraft(lp);
						//Aircraft aircraft = s->controller->m_q.top();

						if(aircraft.m_remaining_dist <= 0)
						{
							s->controller->handle_incoming(lp);

							s->transit_req_accepted++;
							s->transit_processed++;

							s->controller->remove_aircraft(lp);
							//s->controller->m_q.pop();

							s->delay_region = s->delay_region  + (tw_now(lp) - aircraft.m_clock);
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
				else
				{
					s->transit_req_rejected++;
				}

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;

			}
		case ON_THE_AIR:
			{
				//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				s->controller->handle_outgoing(lp);
				//s->controller->m_counter[msg->aircraft.m_id]++;
				
				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				(*(s->counter))[msg->aircraft.m_id] = c;

				int i=0;
				while(i < num_aircraft)
				{
                    const vector<int>& counterRef = *(s->counter);
					int counter = counterRef.at(i);
					//int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}

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
					//assert(next_region == msg->aircraft.m_dest_region);
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

				//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);

				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);
				
				s->controller->add_aircraft(aircraft, lp);
				//s->controller->m_q.push(aircraft);

				if(s->controller->m_q.size() > s->max_queue_size_airport)
					s->max_queue_size_airport = s->controller->m_q.size();

				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					s->controller->handle_incoming(lp);

					s->landing_req_accepted++;
					s->landing_processed++;


					Aircraft aircraft = s->controller->get_aircraft(lp);
					s->controller->remove_aircraft(lp);
					//Aircraft aircraft = s->controller->m_q.top();
					//s->controller->m_q.pop();

					s->delay_airport_land = s->delay_airport_land + (tw_now(lp) - aircraft.m_clock);
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

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

		case LANDING:
			{
				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					if(s->controller->m_q.size() > 0)
					{
						s->controller->handle_incoming(lp);
						s->landing_processed++;

						Aircraft aircraft = s->controller->get_aircraft(lp);
						s->controller->remove_aircraft(lp);						
						//Aircraft aircraft = s->controller->m_q.top();
						//s->controller->m_q.pop();
	
						
						s->delay_airport_land = s->delay_airport_land + (tw_now(lp) - aircraft.m_clock);
						s->cdelay_airport_land += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;
						

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

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}	

		case TAXI_IN:
			{
				//s->controller->m_counter[msg->aircraft.m_id]++;
				
				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				(*(s->counter))[msg->aircraft.m_id] = c;

				int i=0;
				while(i < num_aircraft)
				{
                    const vector<int>& counterRef = *(s->counter);
					int counter = counterRef.at(i);
					//int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}
				
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
				s->controller->handle_outgoing(lp);

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

void event_handler_fw(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
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

				__store__(s->controller->m_q, lp);

				s->controller->add_aircraft(aircraft,lp);
				//s->controller->m_q.push(aircraft);

				int path1=0;

				if(s->controller->m_q.size() > s->max_queue_size_airport)
				{
					path1=1;
					__store__(s->max_queue_size_airport, lp);
					s->max_queue_size_airport = s->controller->m_q.size();
				}

				__store__(path1, lp);
				
				int path2=0;

				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					path2=1;

					s->controller->handle_incoming(lp);

					s->dep_req_accepted++;
					s->dep_processed++;

					Aircraft aircraft = s->controller->get_aircraft(lp);
					s->controller->remove_aircraft(lp);

					//Aircraft aircraft = s->controller->m_q.top();
					//s->controller->m_q.pop();

					__store__(s->delay_airport_dep, lp);
					s->delay_airport_dep += tw_now(lp) - aircraft.m_clock;

					__store__(s->cdelay_airport_dep, lp);					
					s->cdelay_airport_dep += aircraft.m_cdelay;

					aircraft.m_clock = 0;
					aircraft.m_cdelay = 0;
					aircraft.m_delay = 0;

					int to = lp->gid;
					ts = bs_rand_exponential2(s->rn, MEAN_DEQ,lp);

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

				__store__(path2,lp);


				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

		case DEP:
			{
				int path1 = 0;
				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					if(s->controller->m_q.size() > 0)
					{
						path1=1;
						s->controller->handle_incoming(lp);

						s->dep_processed++;

						__store__(s->controller->m_q, lp);

						Aircraft aircraft = s->controller->get_aircraft(lp);
						s->controller->remove_aircraft(lp);

						//Aircraft aircraft = s->controller->m_q.top();
						//s->controller->m_q.pop();

						__store__(s->delay_airport_dep, lp);
						s->delay_airport_dep += tw_now(lp) - aircraft.m_clock;

						__store__(s->cdelay_airport_dep, lp);
						s->cdelay_airport_dep += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;

						int to = lp->gid;
						ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = TAXI_OUT;
						m->aircraft = aircraft;

						tw_event_send(e);
					}
				}

				__store__(path1, lp);

				int path2 = 0;
				if(s->controller->m_q.size() > 0)
				{
					path2 = 1;

					int q_stored=0;
					if(path1) q_stored =1;
					if(!q_stored)__store__(s->controller->m_q, lp);
					s->controller->handle_aircraft(lp);

					__store__(q_stored, lp);					
				}
				
				__store__(path2, lp);

				break;
			}

		case TAXI_OUT:
			{
				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				 (*(s->counter))[msg->aircraft.m_id] = c;

				__store__(s->max_counter_aircraft_id, lp);
				__store__(s->max_counter, lp);

				int i=0;
				while(i < num_aircraft)
				{
					int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}
				 
				int to = lp->gid;				
				ts = bs_rand_exponential2(s->rn, MEAN_TAXI, lp);

				e = tw_event_new(to, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = TAKE_OFF;
				m->aircraft = msg->aircraft;

				tw_event_send(e);

				break;
			}

		case TAKE_OFF:
			{            
				s->controller->handle_outgoing(lp);

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
				ts = bs_rand_exponential2(s->rn, MEAN_TAKE_OFF, lp);

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
				ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);

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
	
				Aircraft msg_aircraft = msg->aircraft;
				msg_aircraft.m_clock = tw_now(lp);

				__store__(s->controller->m_q, lp);
				s->controller->add_aircraft(msg_aircraft, lp);
//				s->controller->m_q.push(msg_aircraft);

				int path1 = 0;
				if(s->controller->m_q.size() > s->max_queue_size_region)
				{
					path1=1;

					__store__(s->max_queue_size_region, lp);
					s->max_queue_size_region = s->controller->m_q.size();
				}
				__store__(path1, lp);

				int path2 = 0;
				int path2_1 = 0;
				if(s->controller->m_current_capacity < s->controller->m_max_capacity)
				{
					path2=1;

					Aircraft aircraft = s->controller->get_aircraft(lp);
//					Aircraft aircraft = s->controller->m_q.top();

					if(aircraft.m_remaining_dist <= 0)
					{
						path2_1=1;

						s->transit_req_accepted++;
						s->transit_processed++;

						s->controller->handle_incoming(lp);

						s->controller->remove_aircraft(lp);
//						s->controller->m_q.pop();

						__store__(s->delay_region, lp);
						s->delay_region += tw_now(lp) - aircraft.m_clock;

						__store__(s->cdelay_region, lp);
						s->cdelay_region += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;

						int to = lp->gid;
						ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT, lp);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = ON_THE_AIR;
						m->aircraft = aircraft;
						m->msg_from = lp->gid;

						tw_event_send(e);
					}
					__store__(path2_1, lp);
				}
				else
				{
					s->transit_req_rejected++;
				}

				__store__(path2, lp);

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}

				break;
			}

			/*
			   Region Controller Handles This Event
			 */

		case TRANSIT:
			{
				int path1 = 0;
				int path1_2 = 0;
				int path1_1 =0;

				if(s->controller->m_current_capacity < s->controller->m_max_capacity)
				{
					path1=1;

					if(s->controller->m_q.size() > 0)
					{
						path1_1=1;
						
						Aircraft aircraft = s->controller->get_aircraft(lp);
//						Aircraft aircraft = s->controller->m_q.top();

						if(aircraft.m_remaining_dist <= 0)
						{
							path1_2=1;

							s->controller->handle_incoming(lp);

							s->transit_req_accepted++;
							s->transit_processed++;

							__store__(s->controller->m_q, lp);
							s->controller->remove_aircraft(lp);
//							s->controller->m_q.pop();

							__store__(s->delay_region, lp);
							s->delay_region += tw_now(lp) - aircraft.m_clock;

							__store__(s->cdelay_region, lp);
							s->cdelay_region += aircraft.m_cdelay;

							aircraft.m_clock = 0;
							aircraft.m_cdelay = 0;
							aircraft.m_delay = 0;

							int to = lp->gid;
							ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT,lp);

							e = tw_event_new(to, ts, lp);

							m = (air_traffic_message*)tw_event_data(e);
							m->type = ON_THE_AIR;
							m->aircraft = aircraft;
							m->msg_from = lp->gid;

							tw_event_send(e);
						}
						__store__(path1_2, lp);
					}
					__store__(path1_1, lp);
				}
				else
				{
					s->transit_req_rejected++;
				}

				__store__(path1, lp);

				int path2=0;
				if(s->controller->m_q.size() > 0)
				{
					path2=1;
					int q_stored = 0;
					if(path1_1 && path1_2) q_stored = 1;
					if(!q_stored)__store__(s->controller->m_q, lp);
					s->controller->handle_aircraft(lp);
					__store__(q_stored, lp);
				}
				__store__(path2, lp);

				break;

			}
		case ON_THE_AIR:
			{
				assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);

				s->controller->handle_outgoing(lp);
				//s->controller->m_counter[msg->aircraft.m_id]++;
				
				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				(*(s->counter))[msg->aircraft.m_id] = c;

				__store__(s->max_counter_aircraft_id, lp);
				__store__(s->max_counter, lp);

				int i=0;
				while(i < num_aircraft)
				{
					int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}

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

				int path1=0;
				if (next_region == msg->aircraft.m_dest_region)
				{
					path1=1;

					int to = msg->aircraft.m_dest_airport;
					ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);

					e = tw_event_new(to, ts, lp);

					double land_time = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
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
					ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT, lp);

					e = tw_event_new(to2, ts, lp);

					m = (air_traffic_message*)tw_event_data(e);
					m->type = TRANSIT;
					m->aircraft = msg->aircraft;

					tw_event_send(e);

				}
				else
				{
					int to = next_region;
					ts = bs_rand_exponential2(s->rn, MEAN_REQ, lp);

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
				
				__store__(path1, lp);

				break;
			}

		case LANDING_REQ:
			{

				assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);

				Aircraft aircraft = msg->aircraft;
				aircraft.m_clock = tw_now(lp);

				__store__(s->controller->m_q, lp);
				s->controller->add_aircraft(aircraft,lp);
//				s->controller->m_q.push(aircraft);

				int path1=0;
				if(s->controller->m_q.size() > s->max_queue_size_airport)
				{
					path1=1;
					__store__(s->max_queue_size_airport, lp);
					s->max_queue_size_airport = s->controller->m_q.size();
				}
				__store__(path1, lp);

				int path2=0;
				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					path2=1;
					s->controller->handle_incoming(lp);

					s->landing_req_accepted++;
					s->landing_processed++;

					Aircraft aircraft = s->controller->get_aircraft(lp);
					s->controller->remove_aircraft(lp);

//					Aircraft aircraft = s->controller->m_q.top();
//					s->controller->m_q.pop();

					__store__(s->delay_airport_land, lp);
					s->delay_airport_land = s->delay_airport_land + (tw_now(lp) - aircraft.m_clock);

					__store__(s->cdelay_airport_land, lp);
					s->cdelay_airport_land += aircraft.m_cdelay;

					aircraft.m_clock = 0;
					aircraft.m_cdelay = 0;
					aircraft.m_delay = 0;

					int to = lp->gid;
					ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);

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

				__store__(path2, lp);

				if(s->controller->m_q.size() > 0)
				{
					s->controller->handle_aircraft(lp);
				}


				break;
			}

		case LANDING:
			{
				int path1=0;
				if (s->controller->m_current_capacity < s->controller->m_max_capacity) 
				{
					if(s->controller->m_q.size() > 0)
					{
						path1=1;
						s->controller->handle_incoming(lp);
						s->landing_processed++;

						__store__(s->controller->m_q, lp);		

						Aircraft aircraft = s->controller->get_aircraft(lp);
						s->controller->remove_aircraft(lp);

//						Aircraft aircraft = s->controller->m_q.top();
//						s->controller->m_q.pop();

						
						__store__(s->delay_airport_land, lp);
						s->delay_airport_land = s->delay_airport_land + (tw_now(lp) - aircraft.m_clock);

						__store__(s->cdelay_airport_land, lp);
						s->cdelay_airport_land += aircraft.m_cdelay;

						aircraft.m_clock = 0;
						aircraft.m_cdelay = 0;
						aircraft.m_delay = 0;
						

						int to = lp->gid;
						ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);

						e = tw_event_new(to, ts, lp);

						m = (air_traffic_message*)tw_event_data(e);
						m->type = TAXI_IN;
						m->aircraft = aircraft;
						m->msg_from = lp->gid;
						tw_event_send(e);				
					}
				}

				__store__(path1, lp);

				int path2=0;
				if(s->controller->m_q.size() > 0)
				{
					path2=1;
					int q_stored=0;
					if(path1)q_stored=1;
					if(!q_stored)__store__(s->controller->m_q, lp);
					s->controller->handle_aircraft(lp);

					__store__(q_stored, lp);
				}
				__store__(path2, lp);

				break;
			}	

		case TAXI_IN:
			{
				//s->controller->m_counter[msg->aircraft.m_id]++;
				
				int c = (*(s->counter))[msg->aircraft.m_id];
				c++;
				(*(s->counter))[msg->aircraft.m_id] = c;

				__store__(s->max_counter_aircraft_id, lp);
				__store__(s->max_counter, lp);

				int i=0;
				while(i < num_aircraft)
				{
					int counter = (*(s->counter))[i];
					if(counter > s->max_counter)
					{
						s->max_counter_aircraft_id = msg->aircraft.m_id;
						s->max_counter = counter;
					}
					i++;
				}
				
				int to = lp->gid;
				ts = bs_rand_exponential2(s->rn, MEAN_ARRIVAL,lp);

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
				s->controller->handle_outgoing(lp);

				int to = lp->gid;
				ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);

				Aircraft aircraft = msg->aircraft;

				double dep_time = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
				dep_time += tw_now(lp);

				int dest_airport = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
				int dest_region = get_region(dest_airport);
				int max_speed = bs_rand_integer2(s->rn, MIN_AIRCRAFT_SPEED, MAX_AIRCRAFT_SPEED, lp);
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
				ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT, lp);

				e = tw_event_new(to2, ts, lp);

				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING;
				m->aircraft = msg->aircraft;

				tw_event_send(e);

				break;
			}
	}
}

void event_handler_rv(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
	tw_stime ts;
	tw_event *e;
	air_traffic_message *m;

	switch(msg->type)
	{
		//        *(int *)bf = (int)0;

		case DEP_REQ:
			{
				
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);

				if(path2)
				{
					bs_rand_rvs(s->rn, lp);
					__restore__(s->cdelay_airport_dep, lp);
					__restore__(s->delay_airport_dep, lp);

					s->dep_processed--;
					s->dep_req_accepted--;
					
					s->controller->m_aircraft_processed--;
					s->controller->m_current_capacity--;
					
				}
				else
				{
					s->dep_req_rejected--;
				}


				int path1=-1;
				__restore__(path1, lp);
				assert(path1>=0);

				if(path1)
				{
					__restore__(s->max_queue_size_airport, lp);
				}

				__restore__(s->controller->m_q, lp);
				
				break;
			}

		case DEP:
			{
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);

				int q_stored_in_path1 = -1;
				if(path2)
				{
					__restore__(q_stored_in_path1, lp);
					assert(q_stored_in_path1 >=0);

					if(!q_stored_in_path1)
					{
						__restore__(s->controller->m_q, lp);
					}
				}
				
				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					bs_rand_rvs(s->rn, lp);
					__restore__(s->cdelay_airport_dep, lp);
					__restore__(s->delay_airport_dep, lp);
					__restore__(s->controller->m_q, lp);
					
					//assert(q_stored_in_path1 == 1);

					s->dep_processed--;

					s->controller->m_aircraft_processed--;
					s->controller->m_current_capacity--;

					
				}

				break;
			}

		case TAXI_OUT:
			{
				bs_rand_rvs(s->rn, lp);
				
				__restore__(s->max_counter, lp);
				__restore__(s->max_counter_aircraft_id, lp);

				(*(s->counter))[msg->aircraft.m_id]--;

				break;
			}

		case TAKE_OFF:
			{            
				bs_rand_rvs(s->rn, lp);				
				bs_rand_rvs(s->rn, lp);
				s->controller->m_current_capacity++;
				break;
			}
			/*
			   Region Controller Handles This Event
			 */
		case TRANSIT_REQ:
			{
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);
	
				if(path2)
				{
					int path2_1=-1;
					__restore__(path2_1, lp);
					assert(path2_1 >=0);

					if(path2_1)
					{
						bs_rand_rvs(s->rn, lp);
						__restore__(s->cdelay_region, lp);
						__restore__(s->delay_region, lp);

						s->controller->m_aircraft_processed--;
						s->controller->m_current_capacity--;
						
						s->transit_req_accepted--;
						s->transit_processed--;

					}

				}
				else
				{
					s->transit_req_rejected--;
				}

				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					__restore__(s->max_queue_size_region, lp);
				}

				__restore__(s->controller->m_q, lp);
				
				break;
			}

			/*
			   Region Controller Handles This Event
			 */

		case TRANSIT:
			{
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);

				int q_stored_in_path1=-1;

				if(path2)
				{
					__restore__(q_stored_in_path1, lp);
					assert(q_stored_in_path1 >=0);
					if(!q_stored_in_path1)
					{
						__restore__(s->controller->m_q, lp);
					}
				}

				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					int path1_1=-1;
					__restore__(path1_1, lp);
					assert(path1_1 >=0);

					if(path1_1)
					{
					
						int path1_2=-1;
						__restore__(path1_2, lp);
						assert(path1_2 >=0);

						if(path1_2)
						{
							bs_rand_rvs(s->rn, lp);
					
							__restore__(s->cdelay_region, lp);
							__restore__(s->delay_region, lp);
							__restore__(s->controller->m_q, lp);	
	
							//assert(q_stored_in_path1 == 1);

							s->transit_req_accepted--;
							s->transit_processed--;

							s->controller->m_aircraft_processed--;
							s->controller->m_current_capacity--;
						}
					}
				}
				else
				{
					s->transit_req_rejected--;
				}

				break;
			}

		case ON_THE_AIR:
			{
				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					bs_rand_rvs(s->rn, lp);
					bs_rand_rvs(s->rn, lp);
					bs_rand_rvs(s->rn, lp);					
				}
				else
				{
					bs_rand_rvs(s->rn, lp);
				
				}

				__restore__(s->max_counter, lp);
				__restore__(s->max_counter_aircraft_id, lp);
				
				(*(s->counter))[msg->aircraft.m_id]--;
				s->controller->m_current_capacity++;

				break;
			}

		case LANDING_REQ:
			{
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);

				if(path2)
				{
					bs_rand_rvs(s->rn, lp);
					__restore__(s->cdelay_airport_land, lp);
					__restore__(s->delay_airport_land, lp);

					s->landing_req_accepted--;
					s->landing_processed--;
					
					s->controller->m_aircraft_processed--;
					s->controller->m_current_capacity--;

				}
				else
				{
					s->landing_req_rejected--;
				}

				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					__restore__(s->max_queue_size_airport, lp);
				}

				__restore__(s->controller->m_q, lp);
				
				break;
			}

		case LANDING:
			{
				int path2=-1;
				__restore__(path2, lp);
				assert(path2 >=0);
				
				int q_stored_in_path1=-1;
				if(path2)
				{
					__restore__(q_stored_in_path1, lp);
					assert(q_stored_in_path1 >= 0);
					if(!q_stored_in_path1)
						__restore__(s->controller->m_q, lp);
				}

				int path1=-1;
				__restore__(path1, lp);
				assert(path1 >=0);

				if(path1)
				{
					bs_rand_rvs(s->rn, lp);
					
					__restore__(s->cdelay_airport_land, lp);
					__restore__(s->delay_airport_land, lp);
					__restore__(s->controller->m_q, lp);

					//assert(q_stored_in_path1 == 1);

					s->landing_processed--;
					
					s->controller->m_aircraft_processed--;
					s->controller->m_current_capacity--;

				}

				break;
			}	

		case TAXI_IN:
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->max_counter, lp);
				__restore__(s->max_counter_aircraft_id, lp);
			
				(*(s->counter))[msg->aircraft.m_id]--;
				
				break;
			}

		case ARRIVAL:
			{
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);

				s->controller->m_current_capacity++;
				break;
			}
	}
}


void final(airport_state * s, tw_lp * lp)
{
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
	
	total_transit_req_accepted += s->transit_req_accepted;
	//cout<<lp->gid<<","<<total_transit_req_accepted<<endl;

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
		(event_f) event_handler_forward,
		(revent_f) event_handler_reverse,
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
	//TWOPT_UINT("loopsize", loop_size, "padding loop size"),
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
			if(lpid >= 20 && lpid <= 185)
			{
				ret = ret - 10;
			}
		}
		else
		{
			if(lpid >= 10 && lpid <= 19)
				ret = ret + 166;
			ret = ret - 176;
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
			if(lpid >= 20 && lpid <= 102)
				ret = ret - 15;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 86;
			if(lpid == 5)
				ret = ret + 84;
			if(lpid >= 7 && lpid <= 9)
				ret = ret + 83;
			if(lpid >= 103 && lpid <= 185)
				ret = ret - 10;

			ret = ret - 88;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 10)
				ret = ret + 166;
			if(lpid == 12)
				ret = ret + 165;          
			if(lpid >= 15 && lpid <= 17)
				ret = ret + 163;                
			if(lpid >= 186 && lpid <= 268)
				ret = ret - 5;

			ret = ret - 176;
		}
		else 
		{
			if(lpid == 11)
				ret = ret + 253;
			if(lpid == 13 || lpid == 14)
				ret = ret + 252;
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 249;

			ret = ret - 264;
		}
	}
	else if (tw_nnodes() == 8) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid == 3)
				ret = ret - 1;
			if(lpid >= 20 && lpid <= 60)
				ret = ret - 17;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 42;
			if(lpid == 5)
				ret = ret + 40;
			if(lpid >= 61 && lpid <= 102)
				ret = ret - 15;

			ret = ret - 44;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 4)
				ret = ret + 84;
			if(lpid == 6)
				ret = ret + 83;          
			if(lpid >= 103 && lpid <= 144)
				ret = ret - 13;                

			ret = ret - 88;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 7 || lpid == 8 || lpid == 9)
				ret = ret + 125;      
			if(lpid >= 145 && lpid <= 185)
				ret = ret - 10;                

			ret = ret - 132;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 10 )
				ret = ret + 166;      
			if(lpid == 12 )
				ret = ret + 165;     			
			if(lpid >= 186 && lpid <= 227)
				ret = ret - 8;                

			ret = ret - 176;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 11 )
				ret = ret + 209;      
			if(lpid == 13 || lpid == 14 )
				ret = ret + 208;     			
			if(lpid >= 228 && lpid <= 268)
				ret = ret - 5;                

			ret = ret - 220;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 15 || lpid  == 16 || lpid ==17)
				ret = ret + 249;        			
			if(lpid >= 269 && lpid <= 309)
				ret = ret - 2;                

			ret = ret - 264;
		}
		else 
		{
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 290;

			ret = ret - 308;
		}
	}

	else if (tw_nnodes() == 16) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20 && lpid <= 39)
				ret = ret - 18;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 20;
			if(lpid == 5)
				ret = ret + 18;
			if(lpid >= 40 && lpid <= 59)
				ret = ret - 16;

			ret = ret - 22;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 3)
				ret = ret + 41;        
			if(lpid >= 60 && lpid <= 80)
				ret = ret - 15;                

			ret = ret - 44;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 4)
				ret = ret + 62;      
			if(lpid >= 81 && lpid <= 101)
				ret = ret - 14;                

			ret = ret - 66;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 6 )
				ret = ret + 82;         			
			if(lpid >= 102 && lpid <= 122)
				ret = ret - 13;                

			ret = ret - 88;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 7 )
				ret = ret + 103;      
			if(lpid >= 123 && lpid <= 143 )
				ret = ret - 12;     			
              

			ret = ret - 110;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 8 )
				ret = ret + 124;        			
			if(lpid >= 144 && lpid <= 164)
				ret = ret - 11;                

			ret = ret - 132;
		}
		else if (g_tw_mynode == 7)
		{
			if(lpid == 9 )
				ret = ret + 145;   			
			if(lpid >= 165 && lpid <= 185)
				ret = ret - 10;

			ret = ret - 154;
		}


		else if (g_tw_mynode == 8)
		{
			if(lpid == 10 )
				ret = ret + 166;   			
			if(lpid >= 186 && lpid <= 206)
				ret = ret - 9;

			ret = ret - 176;
		}
		else if (g_tw_mynode == 9)
		{
			if(lpid == 11 )
				ret = ret + 187;   			
			if(lpid >= 207 && lpid <= 227)
				ret = ret - 8;

			ret = ret - 198;
		}
		else if (g_tw_mynode == 10)
		{
			if(lpid == 12 )
				ret = ret + 208;   			
			if(lpid >= 228 && lpid <= 248)
				ret = ret - 7;

			ret = ret - 220;
		}
		else if (g_tw_mynode == 11)
		{
			if(lpid == 13 )
				ret = ret + 229;   			
			if(lpid >= 249 && lpid <= 269)
				ret = ret - 6;

			ret = ret - 242;
		}
		else if (g_tw_mynode == 12)
		{
			if(lpid == 14 )
				ret = ret + 250;   			
			if(lpid >= 270 && lpid <= 290)
				ret = ret - 5;

			ret = ret - 264;
		}
		else if (g_tw_mynode == 13)
		{
			if(lpid == 15 || lpid == 16)
				ret = ret + 271;   			
			if(lpid >= 291 && lpid <= 310)
				ret = ret - 3;

			ret = ret - 286;
		}
		else if (g_tw_mynode == 14)
		{
			if(lpid == 17 )
				ret = ret + 291;   			
			if(lpid >= 311 && lpid <= 331)
				ret = ret - 2;

			ret = ret - 308;
		}
		else if (g_tw_mynode == 15)
		{
			if(lpid == 18 || lpid ==19 )
				ret = ret + 312;   			

			ret = ret - 330;
		}
	}
	else if (tw_nnodes() == 32) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20 && lpid <= 29)
				ret = ret - 19;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 1)
				ret = ret + 10;
			if(lpid >= 30 && lpid <= 39)
				ret = ret - 18;

			ret = ret - 11;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 2)
				ret = ret + 20;        
			if(lpid >= 40 && lpid <= 49)
				ret = ret - 17;                

			ret = ret - 22;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 3)
				ret = ret + 30;      
			if(lpid >= 50 && lpid <= 59)
				ret = ret - 16;                

			ret = ret - 33;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 4 )
				ret = ret + 40;         			
			if(lpid >= 60 && lpid <= 69)
				ret = ret - 15;                

			ret = ret - 44;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 5 )
				ret = ret + 50;      
			if(lpid >= 70 && lpid <= 79 )
				ret = ret - 14;     			
              
			ret = ret - 55;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 6 )
				ret = ret + 60;        			
			if(lpid >= 80 && lpid <= 89)
				ret = ret - 13;                

			ret = ret - 66;
		}
		else if (g_tw_mynode == 7)
		{
			if(lpid == 7 )
				ret = ret + 70;   			
			if(lpid >= 90 && lpid <= 99)
				ret = ret - 12;

			ret = ret - 77;
		}
		else if (g_tw_mynode == 8)
		{
			if(lpid == 8 )
				ret = ret + 80;   			
			if(lpid >= 100 && lpid <= 109)
				ret = ret - 11;

			ret = ret - 88;
		}
		else if (g_tw_mynode == 9)
		{
			if(lpid == 9 )
				ret = ret + 90;   			
			if(lpid >= 110 && lpid <= 119)
				ret = ret - 10;

			ret = ret - 99;
		}
		else if (g_tw_mynode == 10)
		{
			if(lpid == 10 )
				ret = ret + 100;   			
			if(lpid >= 120 && lpid <= 129)
				ret = ret - 9;

			ret = ret - 110;
		}
		else if (g_tw_mynode == 11)
		{
			if(lpid == 11 )
				ret = ret + 110;   			
			if(lpid >= 130 && lpid <= 139)
				ret = ret - 8;

			ret = ret - 121;
		}
		else if (g_tw_mynode == 12)
		{
			if(lpid == 12 )
				ret = ret + 120;   			
			if(lpid >= 140 && lpid <= 149)
				ret = ret - 7;

			ret = ret - 132;
		}
		else if (g_tw_mynode == 13)
		{
			if(lpid == 13 )
				ret = ret + 130;   			
			if(lpid >= 150 && lpid <= 159)
				ret = ret - 6;

			ret = ret - 143;
		}
		else if (g_tw_mynode == 14)
		{
			if(lpid == 14 )
				ret = ret + 140;   			
			if(lpid >= 160 && lpid <= 169)
				ret = ret - 5;

			ret = ret - 154;
		}
		else if (g_tw_mynode == 15)
		{
			if(lpid == 15 )
				ret = ret + 150;   			
			if(lpid >= 170 && lpid <= 179)
				ret = ret - 4;

			ret = ret - 165;
		}
		else if (g_tw_mynode == 16)
		{
			if(lpid == 16 )
				ret = ret + 160;   			
			if(lpid >= 180 && lpid <= 189)
				ret = ret - 3;

			ret = ret - 176;
		}
		else if (g_tw_mynode == 17)
		{
			if(lpid == 17 )
				ret = ret + 170;   			
			if(lpid >= 190 && lpid <= 199)
				ret = ret - 2;

			ret = ret - 187;
		}
		else if (g_tw_mynode == 18)
		{
			if(lpid == 18 )
				ret = ret + 180;   			
			if(lpid >= 200 && lpid <= 209)
				ret = ret - 1;

			ret = ret - 198;
		}
		else if (g_tw_mynode == 19)
		{
			if(lpid == 19 )
				ret = ret + 190;   			
			ret = ret - 209;
		}
		else if (g_tw_mynode == 20)
		{
			ret = ret - 220;
		}
		else if (g_tw_mynode == 21)
		{
			ret = ret - 231;
		}
		else if (g_tw_mynode == 22)
		{
			ret = ret - 242;
		}
		else if (g_tw_mynode == 23)
		{
			ret = ret - 253;
		}
		else if (g_tw_mynode == 24)
		{
			ret = ret - 264;
		}
		else if (g_tw_mynode == 25)
		{
			ret = ret - 275;
		}
		else if (g_tw_mynode == 26)
		{
			ret = ret - 286;
		}
		else if (g_tw_mynode == 27)
		{
			ret = ret - 297;
		}
		else if (g_tw_mynode == 28)
		{
			ret = ret - 308;
		}
		else if (g_tw_mynode == 29)
		{
			ret = ret - 319;
		}
		else if (g_tw_mynode == 30)
		{
			ret = ret - 330;
		}
		else if (g_tw_mynode == 31)
		{
			ret = ret - 341;
		}
	}
	else 
	{
		std::cout << "Only support upto 32 cores "<<std::endl;;
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
			if(lpid >= 20 && lpid <= 185)
			{
				ret = ret - 10;
			}
		}
		else
		{
			if(lpid >= 10 && lpid <= 19)
				ret = ret + 166;
			ret = ret - 176;
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
			if(lpid >= 20 && lpid <= 102)
				ret = ret - 15;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 86;
			if(lpid == 5)
				ret = ret + 84;
			if(lpid >= 7 && lpid <= 9)
				ret = ret + 83;
			if(lpid >= 103 && lpid <= 185)
				ret = ret - 10;

			ret = ret - 88;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 10)
				ret = ret + 166;
			if(lpid == 12)
				ret = ret + 165;          
			if(lpid >= 15 && lpid <= 17)
				ret = ret + 163;                
			if(lpid >= 186 && lpid <= 268)
				ret = ret - 5;

			ret = ret - 176;
		}
		else 
		{
			if(lpid == 11)
				ret = ret + 253;
			if(lpid == 13 || lpid == 14)
				ret = ret + 252;
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 249;

			ret = ret - 264;
		}
	}
	else if (tw_nnodes() == 8) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid == 3)
				ret = ret - 1;
			if(lpid >= 20 && lpid <= 60)
				ret = ret - 17;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 42;
			if(lpid == 5)
				ret = ret + 40;
			if(lpid >= 61 && lpid <= 102)
				ret = ret - 15;

			ret = ret - 44;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 4)
				ret = ret + 84;
			if(lpid == 6)
				ret = ret + 83;          
			if(lpid >= 103 && lpid <= 144)
				ret = ret - 13;                

			ret = ret - 88;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 7 || lpid == 8 || lpid == 9)
				ret = ret + 125;      
			if(lpid >= 145 && lpid <= 185)
				ret = ret - 10;                

			ret = ret - 132;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 10 )
				ret = ret + 166;      
			if(lpid == 12 )
				ret = ret + 165;     			
			if(lpid >= 186 && lpid <= 227)
				ret = ret - 8;                

			ret = ret - 176;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 11 )
				ret = ret + 209;      
			if(lpid == 13 || lpid == 14 )
				ret = ret + 208;     			
			if(lpid >= 228 && lpid <= 268)
				ret = ret - 5;                

			ret = ret - 220;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 15 || lpid  == 16 || lpid ==17)
				ret = ret + 249;        			
			if(lpid >= 269 && lpid <= 309)
				ret = ret - 2;                

			ret = ret - 264;
		}
		else 
		{
			if(lpid >= 18 && lpid <= 19)
				ret = ret + 290;

			ret = ret - 308;
		}
	}
	else if (tw_nnodes() == 16) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20 && lpid <= 39)
				ret = ret - 18;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 2)
				ret = ret + 20;
			if(lpid == 5)
				ret = ret + 18;
			if(lpid >= 40 && lpid <= 59)
				ret = ret - 16;

			ret = ret - 22;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 3)
				ret = ret + 41;        
			if(lpid >= 60 && lpid <= 80)
				ret = ret - 15;                

			ret = ret - 44;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 4)
				ret = ret + 62;      
			if(lpid >= 81 && lpid <= 101)
				ret = ret - 14;                

			ret = ret - 66;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 6 )
				ret = ret + 82;         			
			if(lpid >= 102 && lpid <= 122)
				ret = ret - 13;                

			ret = ret - 88;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 7 )
				ret = ret + 103;      
			if(lpid >= 123 && lpid <= 143 )
				ret = ret - 12;     			
              

			ret = ret - 110;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 8 )
				ret = ret + 124;        			
			if(lpid >= 144 && lpid <= 164)
				ret = ret - 11;                

			ret = ret - 132;
		}
		else if (g_tw_mynode == 7)
		{
			if(lpid == 9 )
				ret = ret + 145;   			
			if(lpid >= 165 && lpid <= 185)
				ret = ret - 10;

			ret = ret - 154;
		}


		else if (g_tw_mynode == 8)
		{
			if(lpid == 10 )
				ret = ret + 166;   			
			if(lpid >= 186 && lpid <= 206)
				ret = ret - 9;

			ret = ret - 176;
		}
		else if (g_tw_mynode == 9)
		{
			if(lpid == 11 )
				ret = ret + 187;   			
			if(lpid >= 207 && lpid <= 227)
				ret = ret - 8;

			ret = ret - 198;
		}
		else if (g_tw_mynode == 10)
		{
			if(lpid == 12 )
				ret = ret + 208;   			
			if(lpid >= 228 && lpid <= 248)
				ret = ret - 7;

			ret = ret - 220;
		}
		else if (g_tw_mynode == 11)
		{
			if(lpid == 13 )
				ret = ret + 229;   			
			if(lpid >= 249 && lpid <= 269)
				ret = ret - 6;

			ret = ret - 242;
		}
		else if (g_tw_mynode == 12)
		{
			if(lpid == 14 )
				ret = ret + 250;   			
			if(lpid >= 270 && lpid <= 290)
				ret = ret - 5;

			ret = ret - 264;
		}
		else if (g_tw_mynode == 13)
		{
			if(lpid == 15 || lpid == 16)
				ret = ret + 271;   			
			if(lpid >= 291 && lpid <= 310)
				ret = ret - 3;

			ret = ret - 286;
		}
		else if (g_tw_mynode == 14)
		{
			if(lpid == 17 )
				ret = ret + 291;   			
			if(lpid >= 311 && lpid <= 331)
				ret = ret - 2;

			ret = ret - 308;
		}
		else if (g_tw_mynode == 15)
		{
			if(lpid == 18 || lpid ==19 )
				ret = ret + 312;   			

			ret = ret - 330;
		}
	}
	else if (tw_nnodes() == 32) 
	{
		if(g_tw_mynode == 0)
		{
			if(lpid >= 20 && lpid <= 29)
				ret = ret - 19;
		}
		else if (g_tw_mynode == 1)
		{
			if(lpid == 1)
				ret = ret + 10;
			if(lpid >= 30 && lpid <= 39)
				ret = ret - 18;

			ret = ret - 11;
		}
		else if (g_tw_mynode == 2)
		{
			if(lpid == 2)
				ret = ret + 20;        
			if(lpid >= 40 && lpid <= 49)
				ret = ret - 17;                

			ret = ret - 22;
		}
		else if (g_tw_mynode == 3)
		{
			if(lpid == 3)
				ret = ret + 30;      
			if(lpid >= 50 && lpid <= 59)
				ret = ret - 16;                

			ret = ret - 33;
		}
		else if (g_tw_mynode == 4)
		{
			if(lpid == 4 )
				ret = ret + 40;         			
			if(lpid >= 60 && lpid <= 69)
				ret = ret - 15;                

			ret = ret - 44;
		}
		else if (g_tw_mynode == 5)
		{
			if(lpid == 5 )
				ret = ret + 50;      
			if(lpid >= 70 && lpid <= 79 )
				ret = ret - 14;     			
              
			ret = ret - 55;
		}
		else if (g_tw_mynode == 6)
		{
			if(lpid == 6 )
				ret = ret + 60;        			
			if(lpid >= 80 && lpid <= 89)
				ret = ret - 13;                

			ret = ret - 66;
		}
		else if (g_tw_mynode == 7)
		{
			if(lpid == 7 )
				ret = ret + 70;   			
			if(lpid >= 90 && lpid <= 99)
				ret = ret - 12;

			ret = ret - 77;
		}
		else if (g_tw_mynode == 8)
		{
			if(lpid == 8 )
				ret = ret + 80;   			
			if(lpid >= 100 && lpid <= 109)
				ret = ret - 11;

			ret = ret - 88;
		}
		else if (g_tw_mynode == 9)
		{
			if(lpid == 9 )
				ret = ret + 90;   			
			if(lpid >= 110 && lpid <= 119)
				ret = ret - 10;

			ret = ret - 99;
		}
		else if (g_tw_mynode == 10)
		{
			if(lpid == 10 )
				ret = ret + 100;   			
			if(lpid >= 120 && lpid <= 129)
				ret = ret - 9;

			ret = ret - 110;
		}
		else if (g_tw_mynode == 11)
		{
			if(lpid == 11 )
				ret = ret + 110;   			
			if(lpid >= 130 && lpid <= 139)
				ret = ret - 8;

			ret = ret - 121;
		}
		else if (g_tw_mynode == 12)
		{
			if(lpid == 12 )
				ret = ret + 120;   			
			if(lpid >= 140 && lpid <= 149)
				ret = ret - 7;

			ret = ret - 132;
		}
		else if (g_tw_mynode == 13)
		{
			if(lpid == 13 )
				ret = ret + 130;   			
			if(lpid >= 150 && lpid <= 159)
				ret = ret - 6;

			ret = ret - 143;
		}
		else if (g_tw_mynode == 14)
		{
			if(lpid == 14 )
				ret = ret + 140;   			
			if(lpid >= 160 && lpid <= 169)
				ret = ret - 5;

			ret = ret - 154;
		}
		else if (g_tw_mynode == 15)
		{
			if(lpid == 15 )
				ret = ret + 150;   			
			if(lpid >= 170 && lpid <= 179)
				ret = ret - 4;

			ret = ret - 165;
		}
		else if (g_tw_mynode == 16)
		{
			if(lpid == 16 )
				ret = ret + 160;   			
			if(lpid >= 180 && lpid <= 189)
				ret = ret - 3;

			ret = ret - 176;
		}
		else if (g_tw_mynode == 17)
		{
			if(lpid == 17 )
				ret = ret + 170;   			
			if(lpid >= 190 && lpid <= 199)
				ret = ret - 2;

			ret = ret - 187;
		}
		else if (g_tw_mynode == 18)
		{
			if(lpid == 18 )
				ret = ret + 180;   			
			if(lpid >= 200 && lpid <= 209)
				ret = ret - 1;

			ret = ret - 198;
		}
		else if (g_tw_mynode == 19)
		{
			if(lpid == 19 )
				ret = ret + 190;   			
			ret = ret - 209;
		}
		else if (g_tw_mynode == 20)
		{
			ret = ret - 220;
		}
		else if (g_tw_mynode == 21)
		{
			ret = ret - 231;
		}
		else if (g_tw_mynode == 22)
		{
			ret = ret - 242;
		}
		else if (g_tw_mynode == 23)
		{
			ret = ret - 253;
		}
		else if (g_tw_mynode == 24)
		{
			ret = ret - 264;
		}
		else if (g_tw_mynode == 25)
		{
			ret = ret - 275;
		}
		else if (g_tw_mynode == 26)
		{
			ret = ret - 286;
		}
		else if (g_tw_mynode == 27)
		{
			ret = ret - 297;
		}
		else if (g_tw_mynode == 28)
		{
			ret = ret - 308;
		}
		else if (g_tw_mynode == 29)
		{
			ret = ret - 319;
		}
		else if (g_tw_mynode == 30)
		{
			ret = ret - 330;
		}
		else if (g_tw_mynode == 31)
		{
			ret = ret - 341;
		}
	}
	else 
	{
		std::cout << "Only support upto 32 cores "<<std::endl;;
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
	
	int i = 0;
	/*
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	printf("PID %d on %s ready for attach\n", getpid(), hostname);
	fflush(stdout);
	while (0 == i)
		sleep(5);
	*/

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

	//printf("loop size %d \n", loop_size);

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
	else if(airport >=338 && airport <= 351)
	{
		return 19;
	}   
	else
	{
		std::cout<<"airport region failed"<<std::endl;
		assert(false);
	}

}
