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
#include "ross.h" 
#include "rctypes.h" 
#include "backstroke/stl_fwd_rvs_funcs.h" 
int get_region(int airport);
int mapping_to_local_index(int lpid);
int increase_counter(int lipd,int event_type);
int decrease_counter(int lipd,int event_type);
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


void event_handler(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  tw_stime ts;
  tw_event *e;
  air_traffic_message *m;
{
    int __item_selector__ = (int )(msg -> air_traffic_message::type);
    if (__item_selector__ == DEP_REQ) {
      goto LABEL0;
    }
    else {
      if (__item_selector__ == DEP) {
        goto LABEL1;
      }
      else {
        if (__item_selector__ == TAXI_OUT) {
          goto LABEL2;
        }
        else {
          if (__item_selector__ == TAKE_OFF) {
            goto LABEL3;
          }
          else {
            if (__item_selector__ == TRANSIT_REQ) {
              goto LABEL4;
            }
            else {
              if (__item_selector__ == TRANSIT) {
                goto LABEL5;
              }
              else {
                if (__item_selector__ == ON_THE_AIR) {
                  goto LABEL6;
                }
                else {
                  if (__item_selector__ == LANDING_REQ) {
                    goto LABEL7;
                  }
                  else {
                    if (__item_selector__ == LANDING) {
                      goto LABEL8;
                    }
                    else {
                      if (__item_selector__ == TAXI_IN) {
                        goto LABEL9;
                      }
                      else {
                        if (__item_selector__ == ARRIVAL) {
                          goto LABEL10;
                        }
                        else {
                          goto LABEL11;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    LABEL0:
{
//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp4__;
      __temp4__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft (__temp4__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        ( *(s -> airport_state::controller)). handle_incoming (lp);
        s -> airport_state::dep_req_accepted++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        s -> airport_state::delay_airport_dep = ((s -> airport_state::delay_airport_dep) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        s -> airport_state::dep_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft (lp);
      }
      else {
      }
      goto LABEL12;
    }
    LABEL1:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_incoming (lp);
        s -> airport_state::dep_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        s -> airport_state::delay_airport_dep = ((s -> airport_state::delay_airport_dep) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
      }
    }
    else {
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft (lp);
    }
    else {
    }
    goto LABEL12;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAKE_OFF;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL3:
{
      ( *(s -> airport_state::controller)). handle_outgoing (lp);
      int src_region = get_region((lp -> tw_lp::gid));
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        next_region = path. front ();
//assert(next_region == msg->aircraft.m_dest_region);
      }
      float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
      if (distance < 0) {
        distance = 0;
      }
      else {
      }
      int to = next_region;
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TRANSIT_REQ;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
      m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
      tw_event_send(e);
/*
				   Schedule Another Event 
				   Without this event, the simulation will be terminated soon because initial events fed to the engine
				   run out pretty quickly due to the limited resources
				 */
      int to2 = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL4:
{
//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp5__;
      __temp5__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft (__temp5__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_region)) {
        s -> airport_state::max_queue_size_region = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::controller)). handle_incoming (lp);
          ( *(s -> airport_state::controller)). remove_aircraft (lp);
//s->controller->m_q.pop();
          s -> airport_state::delay_region = ((s -> airport_state::delay_region) + (tw_now(lp) - aircraft.Aircraft::m_clock));
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
        }
      }
      else {
        s -> airport_state::transit_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft (lp);
      }
      else {
      }
      goto LABEL12;
    }
    LABEL5:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::controller)). handle_incoming (lp);
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::controller)). remove_aircraft (lp);
//s->controller->m_q.pop();
          s -> airport_state::delay_region = ((s -> airport_state::delay_region) + (tw_now(lp) - aircraft.Aircraft::m_clock));
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
        }
      }
      else {
      }
    }
    else {
      s -> airport_state::transit_req_rejected++;
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft (lp);
    }
    else {
    }
    goto LABEL12;
    LABEL6:
{
//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
      ( *(s -> airport_state::controller)). handle_outgoing (lp);
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int src_region = (lp -> tw_lp::gid);
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        next_region = path. front ();
//assert(next_region == msg->aircraft.m_dest_region);
      }
      if (next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region) {
        int to = msg -> air_traffic_message::aircraft.Aircraft::m_dest_airport;
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        double land_time = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        land_time += tw_now(lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = LANDING_REQ;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::aircraft.Aircraft::m_process_time = land_time;
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
/*
					   Schedule Transit Event
					 */
        int to2 = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
        e = tw_event_new(to2,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TRANSIT;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        tw_event_send(e);
      }
      else {
        int to = next_region;
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_REQ);
        e = tw_event_new(to,ts,lp);
        float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
        if (distance < 0) {
          distance = 0;
        }
        else {
        }
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TRANSIT_REQ;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
        m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      goto LABEL12;
    }
    LABEL7:
{
//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp6__;
      __temp6__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft (__temp6__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        ( *(s -> airport_state::controller)). handle_incoming (lp);
        s -> airport_state::landing_req_accepted++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        s -> airport_state::landing_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft (lp);
      }
      else {
      }
      goto LABEL12;
    }
    LABEL8:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_incoming (lp);
        s -> airport_state::landing_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      else {
      }
    }
    else {
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft (lp);
    }
    else {
    }
    goto LABEL12;
    LABEL9:
{
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = ARRIVAL;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL10:
{
      ( *(s -> airport_state::controller)). handle_outgoing (lp);
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      double dep_time = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      dep_time += tw_now(lp);
      int __temp7__;
      __temp7__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp7__));
      int dest_region = get_region(dest_airport);
      int max_speed = (bs_rand_integer(s -> airport_state::rn,MIN_AIRCRAFT_SPEED,MAX_AIRCRAFT_SPEED));
      int src_region = get_region((lp -> tw_lp::gid));
      aircraft.Aircraft::m_src_airport = (lp -> tw_lp::gid);
      aircraft.Aircraft::m_src_region = src_region;
      aircraft.Aircraft::m_process_time = dep_time;
      aircraft.Aircraft::m_dest_airport = dest_airport;
      aircraft.Aircraft::m_dest_region = dest_region;
      aircraft.Aircraft::m_max_speed = max_speed;
      aircraft.Aircraft::m_speed = max_speed;
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP_REQ;
      m -> air_traffic_message::aircraft = (aircraft);
      tw_event_send(e);
/*
				   Schedule another event
				 */
      int to2 = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = LANDING;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL11:
    LABEL12:;
  }
}

void event_handler_forward(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  int __num0 = 0;
  tw_stime ts;
  tw_event *e;
  air_traffic_message *m;
{
    int __item_selector__ = (int )(msg -> air_traffic_message::type);
    if (__item_selector__ == DEP_REQ) {
      goto LABEL0;
    }
    else {
      __num0 += 2048;
      if (__item_selector__ == DEP) {
        goto LABEL1;
      }
      else {
        __num0 += 1024;
        if (__item_selector__ == TAXI_OUT) {
          goto LABEL2;
        }
        else {
          __num0 += 512;
          if (__item_selector__ == TAKE_OFF) {
            goto LABEL3;
          }
          else {
            __num0 += 256;
            if (__item_selector__ == TRANSIT_REQ) {
              goto LABEL4;
            }
            else {
              __num0 += 128;
              if (__item_selector__ == TRANSIT) {
                goto LABEL5;
              }
              else {
                __num0 += 64;
                if (__item_selector__ == ON_THE_AIR) {
                  goto LABEL6;
                }
                else {
                  __num0 += 32;
                  if (__item_selector__ == LANDING_REQ) {
                    goto LABEL7;
                  }
                  else {
                    __num0 += 16;
                    if (__item_selector__ == LANDING) {
                      goto LABEL8;
                    }
                    else {
                      __num0 += 8;
                      if (__item_selector__ == TAXI_IN) {
                        goto LABEL9;
                      }
                      else {
                        __num0 += 2;
                        if (__item_selector__ == ARRIVAL) {
                          goto LABEL10;
                        }
                        else {
                          __num0 += 1;
                          goto LABEL11;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    LABEL0:
{
//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp4__;
      __temp4__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft_forward (__temp4__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
        __store__(s -> airport_state::max_queue_size_airport,lp);
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
        __num0 += 4;
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
        s -> airport_state::dep_req_accepted++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        __store__(s -> airport_state::delay_airport_dep,lp);
        s -> airport_state::delay_airport_dep = ((s -> airport_state::delay_airport_dep) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        __store__(s -> airport_state::cdelay_airport_dep,lp);
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 2;
        s -> airport_state::dep_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
      }
      else {
        __num0 += 1;
      }
      goto LABEL12;
    }
    LABEL1:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
        s -> airport_state::dep_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        __store__(s -> airport_state::delay_airport_dep,lp);
        s -> airport_state::delay_airport_dep = ((s -> airport_state::delay_airport_dep) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        __store__(s -> airport_state::cdelay_airport_dep,lp);
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 2;
      }
    }
    else {
      __num0 += 4;
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
    }
    else {
      __num0 += 1;
    }
    goto LABEL12;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      __store__(s -> airport_state::max_counter_aircraft_id,lp);
      __store__(s,lp);
      __store__(s -> airport_state::max_counter,lp);
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int to = (lp -> tw_lp::gid);
      __store__(s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAKE_OFF;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL3:
{
      ( *(s -> airport_state::controller)). handle_outgoing_forward (lp);
      int src_region = get_region((lp -> tw_lp::gid));
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        __num0 += 2;
        next_region = path. front ();
//assert(next_region == msg->aircraft.m_dest_region);
      }
      float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
      if (distance < 0) {
        distance = 0;
      }
      else {
        __num0 += 1;
      }
      int to = next_region;
      __store__(s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TRANSIT_REQ;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
      m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
      tw_event_send(e);
/*
				   Schedule Another Event 
				   Without this event, the simulation will be terminated soon because initial events fed to the engine
				   run out pretty quickly due to the limited resources
				 */
      int to2 = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL4:
{
//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp5__;
      __temp5__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft_forward (__temp5__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_region)) {
        __store__(s -> airport_state::max_queue_size_region,lp);
        s -> airport_state::max_queue_size_region = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
        __num0 += 8;
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
          ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//s->controller->m_q.pop();
          __store__(s -> airport_state::delay_region,lp);
          s -> airport_state::delay_region = ((s -> airport_state::delay_region) + (tw_now(lp) - aircraft.Aircraft::m_clock));
          __store__(s -> airport_state::cdelay_region,lp);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          __store__(s -> airport_state::rn,lp);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
          __num0 += 2;
        }
      }
      else {
        __num0 += 4;
        s -> airport_state::transit_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
      }
      else {
        __num0 += 1;
      }
      goto LABEL12;
    }
    LABEL5:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//s->controller->m_q.pop();
          __store__(s -> airport_state::delay_region,lp);
          s -> airport_state::delay_region = ((s -> airport_state::delay_region) + (tw_now(lp) - aircraft.Aircraft::m_clock));
          __store__(s -> airport_state::cdelay_region,lp);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          __store__(s -> airport_state::rn,lp);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
          __num0 += 2;
        }
      }
      else {
        __num0 += 4;
      }
    }
    else {
      __num0 += 8;
      s -> airport_state::transit_req_rejected++;
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
    }
    else {
      __num0 += 1;
    }
    goto LABEL12;
    LABEL6:
{
//assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
      ( *(s -> airport_state::controller)). handle_outgoing_forward (lp);
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      __store__(s,lp);
      __store__(s -> airport_state::max_counter,lp);
      __store__(s -> airport_state::max_counter_aircraft_id,lp);
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int src_region = (lp -> tw_lp::gid);
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        __num0 += 4;
        next_region = path. front ();
//assert(next_region == msg->aircraft.m_dest_region);
      }
      if (next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region) {
        int to = msg -> air_traffic_message::aircraft.Aircraft::m_dest_airport;
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        double land_time = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        land_time += tw_now(lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = LANDING_REQ;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::aircraft.Aircraft::m_process_time = land_time;
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
/*
					   Schedule Transit Event
					 */
        int to2 = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
        e = tw_event_new(to2,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TRANSIT;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 2;
        int to = next_region;
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_REQ);
        e = tw_event_new(to,ts,lp);
        float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
        if (distance < 0) {
          distance = 0;
        }
        else {
          __num0 += 1;
        }
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TRANSIT_REQ;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
        m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      goto LABEL12;
    }
    LABEL7:
{
//assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      class Aircraft __temp6__;
      __temp6__ = (aircraft);
      ( *(s -> airport_state::controller)). add_aircraft_forward (__temp6__,lp);
//s->controller->m_q.push(aircraft);
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
        __store__(s -> airport_state::max_queue_size_airport,lp);
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
      }
      else {
        __num0 += 4;
      }
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
        s -> airport_state::landing_req_accepted++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        __store__(s -> airport_state::delay_airport_land,lp);
        s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        __store__(s -> airport_state::cdelay_airport_land,lp);
        s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 2;
        s -> airport_state::landing_req_rejected++;
      }
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
      }
      else {
        __num0 += 1;
      }
      goto LABEL12;
    }
    LABEL8:
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
      if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
        ( *(s -> airport_state::controller)). handle_incoming_forward (lp);
        s -> airport_state::landing_processed++;
        class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
        ( *(s -> airport_state::controller)). remove_aircraft_forward (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
        __store__(s -> airport_state::delay_airport_land,lp);
        s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
        __store__(s -> airport_state::cdelay_airport_land,lp);
        s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__(s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      else {
        __num0 += 2;
      }
    }
    else {
      __num0 += 4;
    }
    if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
      ( *(s -> airport_state::controller)). handle_aircraft_forward (lp);
    }
    else {
      __num0 += 1;
    }
    goto LABEL12;
    LABEL9:
{
//s->controller->m_counter[msg->aircraft.m_id]++;
      int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      c++;
      ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
      int i = 0;
      __store__(s -> airport_state::max_counter,lp);
      __store__(s,lp);
      __store__(s -> airport_state::max_counter_aircraft_id,lp);
      while(i < num_aircraft){
        const class std::vector< int  , std::allocator< int  >  > &counterRef =  *(s -> airport_state::counter);
        int counter = counterRef. at (i);
//int counter = (*(s->counter))[i];
        if (counter > (s -> airport_state::max_counter)) {
          s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
          s -> airport_state::max_counter = counter;
        }
        else {
        }
        i++;
      }
      int to = (lp -> tw_lp::gid);
      __store__(s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = ARRIVAL;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL10:
{
      ( *(s -> airport_state::controller)). handle_outgoing_forward (lp);
      int to = (lp -> tw_lp::gid);
      __store__(s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      double dep_time = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      dep_time += tw_now(lp);
      int __temp7__;
      __temp7__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp7__));
      int dest_region = get_region(dest_airport);
      int max_speed = (bs_rand_integer(s -> airport_state::rn,MIN_AIRCRAFT_SPEED,MAX_AIRCRAFT_SPEED));
      int src_region = get_region((lp -> tw_lp::gid));
      aircraft.Aircraft::m_src_airport = (lp -> tw_lp::gid);
      aircraft.Aircraft::m_src_region = src_region;
      aircraft.Aircraft::m_process_time = dep_time;
      aircraft.Aircraft::m_dest_airport = dest_airport;
      aircraft.Aircraft::m_dest_region = dest_region;
      aircraft.Aircraft::m_max_speed = max_speed;
      aircraft.Aircraft::m_speed = max_speed;
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP_REQ;
      m -> air_traffic_message::aircraft = (aircraft);
      tw_event_send(e);
/*
				   Schedule another event
				 */
      int to2 = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = LANDING;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL12;
    }
    LABEL11:
    LABEL12:;
  }
  __store__(__num0,lp);
}

void event_handler_reverse(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  int __num0;
  __restore__(__num0,lp);
  int c;
  if ((__num0 & 3584) == 3072) {
    __restore__(s -> airport_state::rn,lp);
    __restore__(s -> airport_state::max_counter,lp);
    __restore__(s,lp);
    __restore__(s -> airport_state::max_counter_aircraft_id,lp);
    c = ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id];
    --c;
    ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
  }
  else {
    if ((__num0 & 4090) == 4088) {
      __restore__(s -> airport_state::rn,lp);
      __restore__(s -> airport_state::max_counter_aircraft_id,lp);
      __restore__(s,lp);
      __restore__(s -> airport_state::max_counter,lp);
      c = ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id];
      --c;
      ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
    }
    else {
      if ((__num0 & 4091) == 4091) {
      }
      else {
        if ((__num0 & 3840) == 3584) {
          __restore__(s -> airport_state::rn,lp);
          ( *(s -> airport_state::controller)). handle_outgoing_reverse (lp);
        }
        else {
          if ((__num0 & 3072) == 2048) {
            if ((__num0 & 3073) == 2048) {
              ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
            }
            else {
            }
            if ((__num0 & 3076) == 2048) {
              if ((__num0 & 3078) == 2050) {
              }
              else {
                __restore__(s -> airport_state::rn,lp);
                __restore__(s -> airport_state::cdelay_airport_dep,lp);
                __restore__(s -> airport_state::delay_airport_dep,lp);
                ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                --s -> airport_state::dep_processed;
                ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
              }
            }
            else {
            }
          }
          else {
            if ((__num0 & 4088) == 4080) {
              if ((__num0 & 4089) == 4080) {
                ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
              }
              else {
              }
              if ((__num0 & 4092) == 4080) {
                if ((__num0 & 4094) == 4082) {
                }
                else {
                  __restore__(s -> airport_state::rn,lp);
                  __restore__(s -> airport_state::cdelay_airport_land,lp);
                  __restore__(s -> airport_state::delay_airport_land,lp);
                  ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                  --s -> airport_state::landing_processed;
                  ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
                }
              }
              else {
              }
            }
            else {
              if ((__num0 & 4064) == 4032) {
                if ((__num0 & 4066) == 4032) {
                  __restore__(s -> airport_state::rn,lp);
                }
                else {
                  __restore__(s -> airport_state::rn,lp);
                }
                __restore__(s -> airport_state::max_counter_aircraft_id,lp);
                __restore__(s -> airport_state::max_counter,lp);
                __restore__(s,lp);
                c = ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id];
                --c;
                ( *s -> airport_state::counter)[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
                ( *(s -> airport_state::controller)). handle_outgoing_reverse (lp);
              }
              else {
                if ((__num0 & 4032) == 3968) {
                  if ((__num0 & 4033) == 3968) {
                    ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
                  }
                  else {
                  }
                  if ((__num0 & 4040) == 3968) {
                    if ((__num0 & 4044) == 3968) {
                      if ((__num0 & 4046) == 3970) {
                      }
                      else {
                        __restore__(s -> airport_state::rn,lp);
                        __restore__(s -> airport_state::cdelay_region,lp);
                        __restore__(s -> airport_state::delay_region,lp);
                        ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                        --s -> airport_state::transit_processed;
                        --s -> airport_state::transit_req_accepted;
                        ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
                      }
                    }
                    else {
                    }
                  }
                  else {
                    --s -> airport_state::transit_req_rejected;
                  }
                }
                else {
                  if ((__num0 & 4080) == 4064) {
                    if ((__num0 & 4081) == 4064) {
                      ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
                    }
                    else {
                    }
                    if ((__num0 & 4082) == 4064) {
                      __restore__(s -> airport_state::rn,lp);
                      __restore__(s -> airport_state::cdelay_airport_land,lp);
                      __restore__(s -> airport_state::delay_airport_land,lp);
                      ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                      --s -> airport_state::landing_processed;
                      --s -> airport_state::landing_req_accepted;
                      ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
                    }
                    else {
                      --s -> airport_state::landing_req_rejected;
                    }
                    if ((__num0 & 4084) == 4068) {
                    }
                    else {
                      __restore__(s -> airport_state::max_queue_size_airport,lp);
                    }
                    ( *(s -> airport_state::controller)). add_aircraft_reverse (lp);
                  }
                  else {
                    if ((__num0 & 3968) == 3840) {
                      if ((__num0 & 3969) == 3840) {
                        ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
                      }
                      else {
                      }
                      if ((__num0 & 3972) == 3840) {
                        if ((__num0 & 3974) == 3842) {
                        }
                        else {
                          __restore__(s -> airport_state::rn,lp);
                          __restore__(s -> airport_state::cdelay_region,lp);
                          __restore__(s -> airport_state::delay_region,lp);
                          ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                          ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
                          --s -> airport_state::transit_processed;
                          --s -> airport_state::transit_req_accepted;
                        }
                      }
                      else {
                        --s -> airport_state::transit_req_rejected;
                      }
                      if ((__num0 & 3976) == 3848) {
                      }
                      else {
                        __restore__(s -> airport_state::max_queue_size_region,lp);
                      }
                      ( *(s -> airport_state::controller)). add_aircraft_reverse (lp);
                    }
                    else {
                      if ((__num0 & 2048) == 0) {
                        if ((__num0 & 2049) == 0) {
                          ( *(s -> airport_state::controller)). handle_aircraft_reverse (lp);
                        }
                        else {
                        }
                        if ((__num0 & 2050) == 2) {
                          --s -> airport_state::dep_req_rejected;
                        }
                        else {
                          __restore__(s -> airport_state::rn,lp);
                          __restore__(s -> airport_state::cdelay_airport_dep,lp);
                          __restore__(s -> airport_state::delay_airport_dep,lp);
                          ( *(s -> airport_state::controller)). remove_aircraft_reverse (lp);
                          --s -> airport_state::dep_processed;
                          --s -> airport_state::dep_req_accepted;
                          ( *(s -> airport_state::controller)). handle_incoming_reverse (lp);
                        }
                        if ((__num0 & 2052) == 4) {
                        }
                        else {
                          __restore__(s -> airport_state::max_queue_size_airport,lp);
                        }
                        ( *(s -> airport_state::controller)). add_aircraft_reverse (lp);
                      }
                      else {
                        __restore__(s -> airport_state::rn,lp);
                        ( *(s -> airport_state::controller)). handle_outgoing_reverse (lp);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void event_handler_fw(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  tw_stime ts;
  tw_event *e;
  air_traffic_message *m;
  switch((msg -> air_traffic_message::type)){
//        *(int *)bf = (int)0;
    case DEP_REQ:
{
{
//        (__builtin_expect((!((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1))),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1012,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1") : ((void )0);
        class Aircraft aircraft(msg -> air_traffic_message::aircraft);
        aircraft.Aircraft::m_clock = tw_now(lp);
        __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        ( *(s -> airport_state::controller)). add_aircraft ((aircraft),lp);
//s->controller->m_q.push(aircraft);
        int path1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
          path1 = 1;
          __store__((s -> airport_state::max_queue_size_airport),lp);
          s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
        }
        else {
        }
        __store__(path1,lp);
        int path2 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          path2 = 1;
          ( *(s -> airport_state::controller)). handle_incoming (lp);
          s -> airport_state::dep_req_accepted++;
          s -> airport_state::dep_processed++;
          class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
          ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
          ::__store__((s -> airport_state::delay_airport_dep),lp);
          s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
          __store__((s -> airport_state::cdelay_airport_dep),lp);
          s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = TAXI_OUT;
          m -> air_traffic_message::aircraft = (aircraft);
          tw_event_send(e);
        }
        else {
          s -> airport_state::dep_req_rejected++;
        }
        __store__(path2,lp);
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
        }
        else {
        }
        break; 
      }
    }
    case DEP:
{
{
        int path1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
            path1 = 1;
            ( *(s -> airport_state::controller)). handle_incoming (lp);
            s -> airport_state::dep_processed++;
            __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
            class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
            ( *(s -> airport_state::controller)). remove_aircraft (lp);
//Aircraft aircraft = s->controller->m_q.top();
//s->controller->m_q.pop();
            ::__store__((s -> airport_state::delay_airport_dep),lp);
            s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
            __store__((s -> airport_state::cdelay_airport_dep),lp);
            s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
            aircraft.Aircraft::m_clock = 0;
            aircraft.Aircraft::m_cdelay = 0;
            aircraft.Aircraft::m_delay = 0;
            int to = (lp -> tw_lp::gid);
            ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
            e = tw_event_new(to,ts,lp);
            m = ((air_traffic_message *)(tw_event_data(e)));
            m -> air_traffic_message::type = TAXI_OUT;
            m -> air_traffic_message::aircraft = (aircraft);
            tw_event_send(e);
          }
          else {
          }
        }
        else {
        }
        __store__(path1,lp);
        int path2 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          path2 = 1;
          int q_stored = 0;
          if (path1) {
            q_stored = 1;
          }
          else {
          }
          if (!q_stored) {
            __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
          __store__(q_stored,lp);
        }
        else {
        }
        __store__(path2,lp);
        break; 
      }
    }
    case TAXI_OUT:
{
{
        int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
        c++;
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
        __store__((s -> airport_state::max_counter_aircraft_id),lp);
        __store__((s -> airport_state::max_counter),lp);
        int i = 0;
        while(i < num_aircraft){
          int counter = ( *(s -> airport_state::counter))[i];
          if (counter > (s -> airport_state::max_counter)) {
            s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
            s -> airport_state::max_counter = counter;
          }
          else {
          }
          i++;
        }
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_TAXI,lp);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAKE_OFF;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        tw_event_send(e);
        break; 
      }
    }
    case TAKE_OFF:
{
{
        ( *(s -> airport_state::controller)). handle_outgoing (lp);
        int src_region = get_region((lp -> tw_lp::gid));
        int next_region = 0;
        class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if (path. size () != 1) {
          path. pop_front ();
          next_region = path. front ();
        }
        else {
          next_region = path. front ();;
//          (__builtin_expect((!(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1201,"next_region == msg->aircraft.m_dest_region") : ((void )0);
        }
        float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
        if (distance < 0) {
          distance = 0;
        }
        else {
        }
        int to = next_region;
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_TAKE_OFF,lp);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TRANSIT_REQ;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
        m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
        tw_event_send(e);
/*
				   Schedule Another Event 
				   Without this event, the simulation will be terminated soon because initial events fed to the engine
				   run out pretty quickly due to the limited resources
				 */
        int to2 = (lp -> tw_lp::gid);
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
        e = tw_event_new(to2,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = DEP;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        tw_event_send(e);
        break; 
      }
    }
/*
			   Region Controller Handles This Event
			 */
    case TRANSIT_REQ:
{
{
//        (__builtin_expect((!((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1244,"lp->gid < NUMBER_OF_REGION_CONTROLLER") : ((void )0);
        class Aircraft msg_aircraft(msg -> air_traffic_message::aircraft);
        msg_aircraft.Aircraft::m_clock = tw_now(lp);
        __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        ( *(s -> airport_state::controller)). add_aircraft ((msg_aircraft),lp);
//				s->controller->m_q.push(msg_aircraft);
        int path1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_region)) {
          path1 = 1;
          __store__((s -> airport_state::max_queue_size_region),lp);
          s -> airport_state::max_queue_size_region = (( *(s -> airport_state::controller)).Controller::m_q. size ());
        }
        else {
        }
        __store__(path1,lp);
        int path2 = 0;
        int path2_1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          path2 = 1;
          class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//					Aircraft aircraft = s->controller->m_q.top();
          if (aircraft.Aircraft::m_remaining_dist <= 0) {
            path2_1 = 1;
            s -> airport_state::transit_req_accepted++;
            s -> airport_state::transit_processed++;
            ( *(s -> airport_state::controller)). handle_incoming (lp);
            ( *(s -> airport_state::controller)). remove_aircraft (lp);
//						s->controller->m_q.pop();
            ::__store__((s -> airport_state::delay_region),lp);
            s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
            __store__((s -> airport_state::cdelay_region),lp);
            s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
            aircraft.Aircraft::m_clock = 0;
            aircraft.Aircraft::m_cdelay = 0;
            aircraft.Aircraft::m_delay = 0;
            int to = (lp -> tw_lp::gid);
            ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_FLIGHT,lp);
            e = tw_event_new(to,ts,lp);
            m = ((air_traffic_message *)(tw_event_data(e)));
            m -> air_traffic_message::type = ON_THE_AIR;
            m -> air_traffic_message::aircraft = (aircraft);
            m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
            tw_event_send(e);
          }
          else {
          }
          __store__(path2_1,lp);
        }
        else {
          s -> airport_state::transit_req_rejected++;
        }
        __store__(path2,lp);
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
        }
        else {
        }
        break; 
      }
    }
/*
			   Region Controller Handles This Event
			 */
    case TRANSIT:
{
{
        int path1 = 0;
        int path1_2 = 0;
        int path1_1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          path1 = 1;
          if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
            path1_1 = 1;
            class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
//						Aircraft aircraft = s->controller->m_q.top();
            if (aircraft.Aircraft::m_remaining_dist <= 0) {
              path1_2 = 1;
              ( *(s -> airport_state::controller)). handle_incoming (lp);
              s -> airport_state::transit_req_accepted++;
              s -> airport_state::transit_processed++;
              __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
              ( *(s -> airport_state::controller)). remove_aircraft (lp);
//							s->controller->m_q.pop();
              ::__store__((s -> airport_state::delay_region),lp);
              s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
              __store__((s -> airport_state::cdelay_region),lp);
              s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
              aircraft.Aircraft::m_clock = 0;
              aircraft.Aircraft::m_cdelay = 0;
              aircraft.Aircraft::m_delay = 0;
              int to = (lp -> tw_lp::gid);
              ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_FLIGHT,lp);
              e = tw_event_new(to,ts,lp);
              m = ((air_traffic_message *)(tw_event_data(e)));
              m -> air_traffic_message::type = ON_THE_AIR;
              m -> air_traffic_message::aircraft = (aircraft);
              m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
              tw_event_send(e);
            }
            else {
            }
            __store__(path1_2,lp);
          }
          else {
          }
          __store__(path1_1,lp);
        }
        else {
          s -> airport_state::transit_req_rejected++;
        }
        __store__(path1,lp);
        int path2 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          path2 = 1;
          int q_stored = 0;
          if (path1_1 && path1_2) {
            q_stored = 1;
          }
          else {
          }
          if (!q_stored) {
            __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
          __store__(q_stored,lp);
        }
        else {
        }
        __store__(path2,lp);
        break; 
      }
    }
    case ON_THE_AIR:
{
{
//        (__builtin_expect((!((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1407,"lp->gid < NUMBER_OF_REGION_CONTROLLER") : ((void )0);
        ( *(s -> airport_state::controller)). handle_outgoing (lp);
//s->controller->m_counter[msg->aircraft.m_id]++;
        int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
        c++;
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
        __store__((s -> airport_state::max_counter_aircraft_id),lp);
        __store__((s -> airport_state::max_counter),lp);
        int i = 0;
        while(i < num_aircraft){
          int counter = ( *(s -> airport_state::counter))[i];
          if (counter > (s -> airport_state::max_counter)) {
            s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
            s -> airport_state::max_counter = counter;
          }
          else {
          }
          i++;
        }
        int src_region = (lp -> tw_lp::gid);
        int next_region = 0;
        class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if (path. size () != 1) {
          path. pop_front ();
          next_region = path. front ();
        }
        else {
          next_region = path. front ();;
//          (__builtin_expect((!(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1443,"next_region == msg->aircraft.m_dest_region") : ((void )0);
        }
        int path1 = 0;
        if (next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region) {
          path1 = 1;
          int to = msg -> air_traffic_message::aircraft.Aircraft::m_dest_airport;
          ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
          e = tw_event_new(to,ts,lp);
          double land_time = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
          land_time += tw_now(lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = LANDING_REQ;
          m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
          m -> air_traffic_message::aircraft.Aircraft::m_process_time = land_time;
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
/*
					   Schedule Transit Event
					 */
          int to2 = (lp -> tw_lp::gid);
          ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_FLIGHT,lp);
          e = tw_event_new(to2,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = TRANSIT;
          m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
          tw_event_send(e);
        }
        else {
          int to = next_region;
          ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_REQ,lp);
          e = tw_event_new(to,ts,lp);
          float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
          if (distance < 0) {
            distance = 0;
          }
          else {
          }
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = TRANSIT_REQ;
          m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
          m -> air_traffic_message::aircraft.Aircraft::m_process_time = distance;
          m -> air_traffic_message::aircraft.Aircraft::m_remaining_dist = distance;
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        __store__(path1,lp);
        break; 
      }
    }
    case LANDING_REQ:
{
{
//        (__builtin_expect((!((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1))),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1512,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1") : ((void )0);
        class Aircraft aircraft(msg -> air_traffic_message::aircraft);
        aircraft.Aircraft::m_clock = tw_now(lp);
        __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        ( *(s -> airport_state::controller)). add_aircraft ((aircraft),lp);
//				s->controller->m_q.push(aircraft);
        int path1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > (s -> airport_state::max_queue_size_airport)) {
          path1 = 1;
          __store__((s -> airport_state::max_queue_size_airport),lp);
          s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::controller)).Controller::m_q. size ());
        }
        else {
        }
        __store__(path1,lp);
        int path2 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          path2 = 1;
          ( *(s -> airport_state::controller)). handle_incoming (lp);
          s -> airport_state::landing_req_accepted++;
          s -> airport_state::landing_processed++;
          class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
          ( *(s -> airport_state::controller)). remove_aircraft (lp);
//					Aircraft aircraft = s->controller->m_q.top();
//					s->controller->m_q.pop();
          ::__store__((s -> airport_state::delay_airport_land),lp);
          s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
          __store__((s -> airport_state::cdelay_airport_land),lp);
          s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = TAXI_IN;
          m -> air_traffic_message::aircraft = (aircraft);
          tw_event_send(e);
        }
        else {
          s -> airport_state::landing_req_rejected++;
        }
        __store__(path2,lp);
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
        }
        else {
        }
        break; 
      }
    }
    case LANDING:
{
{
        int path1 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
          if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
            path1 = 1;
            ( *(s -> airport_state::controller)). handle_incoming (lp);
            s -> airport_state::landing_processed++;
            __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
            class Aircraft aircraft = ( *(s -> airport_state::controller)). get_aircraft (lp);
            ( *(s -> airport_state::controller)). remove_aircraft (lp);
//						Aircraft aircraft = s->controller->m_q.top();
//						s->controller->m_q.pop();
            ::__store__((s -> airport_state::delay_airport_land),lp);
            s -> airport_state::delay_airport_land = ((s -> airport_state::delay_airport_land) + (tw_now(lp) - aircraft.Aircraft::m_clock));
            __store__((s -> airport_state::cdelay_airport_land),lp);
            s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
            aircraft.Aircraft::m_clock = 0;
            aircraft.Aircraft::m_cdelay = 0;
            aircraft.Aircraft::m_delay = 0;
            int to = (lp -> tw_lp::gid);
            ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
            e = tw_event_new(to,ts,lp);
            m = ((air_traffic_message *)(tw_event_data(e)));
            m -> air_traffic_message::type = TAXI_IN;
            m -> air_traffic_message::aircraft = (aircraft);
            m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
            tw_event_send(e);
          }
          else {
          }
        }
        else {
        }
        __store__(path1,lp);
        int path2 = 0;
        if (( *(s -> airport_state::controller)).Controller::m_q. size () > 0) {
          path2 = 1;
          int q_stored = 0;
          if (path1) {
            q_stored = 1;
          }
          else {
          }
          if (!q_stored) {
            __store__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
          ( *(s -> airport_state::controller)). handle_aircraft (lp);
          __store__(q_stored,lp);
        }
        else {
        }
        __store__(path2,lp);
        break; 
      }
    }
    case TAXI_IN:
{
{
//s->controller->m_counter[msg->aircraft.m_id]++;
        int c = ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id];
        c++;
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id] = c;
        __store__((s -> airport_state::max_counter_aircraft_id),lp);
        __store__((s -> airport_state::max_counter),lp);
        int i = 0;
        while(i < num_aircraft){
          int counter = ( *(s -> airport_state::counter))[i];
          if (counter > (s -> airport_state::max_counter)) {
            s -> airport_state::max_counter_aircraft_id = msg -> air_traffic_message::aircraft.Aircraft::m_id;
            s -> airport_state::max_counter = counter;
          }
          else {
          }
          i++;
        }
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_ARRIVAL,lp);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = ARRIVAL;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
        break; 
      }
    }
    case ARRIVAL:
{
{
        ( *(s -> airport_state::controller)). handle_outgoing (lp);
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
        class Aircraft aircraft(msg -> air_traffic_message::aircraft);
        double dep_time = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
        dep_time += tw_now(lp);
        int dest_airport = (bs_rand_integer2(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,(NUMBER_OF_LP - 1),lp));
        int dest_region = get_region(dest_airport);
        int max_speed = (bs_rand_integer2(s -> airport_state::rn,MIN_AIRCRAFT_SPEED,MAX_AIRCRAFT_SPEED,lp));
        int src_region = get_region((lp -> tw_lp::gid));
        aircraft.Aircraft::m_src_airport = (lp -> tw_lp::gid);
        aircraft.Aircraft::m_src_region = src_region;
        aircraft.Aircraft::m_process_time = dep_time;
        aircraft.Aircraft::m_dest_airport = dest_airport;
        aircraft.Aircraft::m_dest_region = dest_region;
        aircraft.Aircraft::m_max_speed = max_speed;
        aircraft.Aircraft::m_speed = max_speed;
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = DEP_REQ;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
/*
				   Schedule another event
				 */
        int to2 = (lp -> tw_lp::gid);
        ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_FLIGHT,lp);
        e = tw_event_new(to2,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = LANDING;
        m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
        tw_event_send(e);
        break; 
      }
    }
  }
}

void event_handler_rv(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  tw_stime ts;
  tw_event *e;
  air_traffic_message *m;
  switch((msg -> air_traffic_message::type)){
//        *(int *)bf = (int)0;
    case DEP_REQ:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1749,"path2 >=0") : ((void )0);
        if (path2) {
          bs_rand_rvs(s -> airport_state::rn,lp);
          __restore__(s -> airport_state::cdelay_airport_dep,lp);
          ::__restore__(s -> airport_state::delay_airport_dep,lp);
          s -> airport_state::dep_processed--;
          s -> airport_state::dep_req_accepted--;
          ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
          ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
        }
        else {
          s -> airport_state::dep_req_rejected--;
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1772,"path1>=0") : ((void )0);
        if (path1) {
          __restore__(s -> airport_state::max_queue_size_airport,lp);
        }
        else {
        }
        __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        break; 
      }
    }
    case DEP:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1788,"path2 >=0") : ((void )0);
        int q_stored_in_path1 = -1;
        if (path2) {
          __restore__(q_stored_in_path1,lp);
//          (__builtin_expect((!(q_stored_in_path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1794,"q_stored_in_path1 >=0") : ((void )0);
          if (!q_stored_in_path1) {
            __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
        }
        else {
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1804,"path1 >=0") : ((void )0);
        if (path1) {
          bs_rand_rvs(s -> airport_state::rn,lp);
          __restore__(s -> airport_state::cdelay_airport_dep,lp);
          ::__restore__(s -> airport_state::delay_airport_dep,lp);
          __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
//assert(q_stored_in_path1 == 1);
          s -> airport_state::dep_processed--;
          ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
          ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
        }
        else {
        }
        break; 
      }
    }
    case TAXI_OUT:
{
{
        bs_rand_rvs(s -> airport_state::rn,lp);
        __restore__(s -> airport_state::max_counter,lp);
        __restore__(s -> airport_state::max_counter_aircraft_id,lp);
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]--;
        break; 
      }
    }
    case TAKE_OFF:
{
{
        bs_rand_rvs(s -> airport_state::rn,lp);
        bs_rand_rvs(s -> airport_state::rn,lp);
        ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
        break; 
      }
    }
/*
			   Region Controller Handles This Event
			 */
    case TRANSIT_REQ:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1852,"path2 >=0") : ((void )0);
        if (path2) {
          int path2_1 = -1;
          __restore__(path2_1,lp);
//          (__builtin_expect((!(path2_1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1858,"path2_1 >=0") : ((void )0);
          if (path2_1) {
            bs_rand_rvs(s -> airport_state::rn,lp);
            __restore__(s -> airport_state::cdelay_region,lp);
            ::__restore__(s -> airport_state::delay_region,lp);
            ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
            ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
            s -> airport_state::transit_req_accepted--;
            s -> airport_state::transit_processed--;
          }
          else {
          }
        }
        else {
          s -> airport_state::transit_req_rejected--;
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1882,"path1 >=0") : ((void )0);
        if (path1) {
          __restore__(s -> airport_state::max_queue_size_region,lp);
        }
        else {
        }
        __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        break; 
      }
    }
/*
			   Region Controller Handles This Event
			 */
    case TRANSIT:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1902,"path2 >=0") : ((void )0);
        int q_stored_in_path1 = -1;
        if (path2) {
          __restore__(q_stored_in_path1,lp);
//          (__builtin_expect((!(q_stored_in_path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1909,"q_stored_in_path1 >=0") : ((void )0);
          if (!q_stored_in_path1) {
            __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
        }
        else {
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1918,"path1 >=0") : ((void )0);
        if (path1) {
          int path1_1 = -1;
          __restore__(path1_1,lp);
//          (__builtin_expect((!(path1_1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1924,"path1_1 >=0") : ((void )0);
          if (path1_1) {
            int path1_2 = -1;
            __restore__(path1_2,lp);
//            (__builtin_expect((!(path1_2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1931,"path1_2 >=0") : ((void )0);
            if (path1_2) {
              bs_rand_rvs(s -> airport_state::rn,lp);
              __restore__(s -> airport_state::cdelay_region,lp);
              ::__restore__(s -> airport_state::delay_region,lp);
              __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
//assert(q_stored_in_path1 == 1);
              s -> airport_state::transit_req_accepted--;
              s -> airport_state::transit_processed--;
              ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
              ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
            }
            else {
            }
          }
          else {
          }
        }
        else {
          s -> airport_state::transit_req_rejected--;
        }
        break; 
      }
    }
    case ON_THE_AIR:
{
{
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1963,"path1 >=0") : ((void )0);
        if (path1) {
          bs_rand_rvs(s -> airport_state::rn,lp);
          bs_rand_rvs(s -> airport_state::rn,lp);
          bs_rand_rvs(s -> airport_state::rn,lp);
        }
        else {
          bs_rand_rvs(s -> airport_state::rn,lp);
        }
        __restore__(s -> airport_state::max_counter,lp);
        __restore__(s -> airport_state::max_counter_aircraft_id,lp);
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]--;
        ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
        break; 
      }
    }
    case LANDING_REQ:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1990,"path2 >=0") : ((void )0);
        if (path2) {
          bs_rand_rvs(s -> airport_state::rn,lp);
          __restore__(s -> airport_state::cdelay_airport_land,lp);
          ::__restore__(s -> airport_state::delay_airport_land,lp);
          s -> airport_state::landing_req_accepted--;
          s -> airport_state::landing_processed--;
          ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
          ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
        }
        else {
          s -> airport_state::landing_req_rejected--;
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2012,"path1 >=0") : ((void )0);
        if (path1) {
          __restore__(s -> airport_state::max_queue_size_airport,lp);
        }
        else {
        }
        __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
        break; 
      }
    }
    case LANDING:
{
{
        int path2 = -1;
        __restore__(path2,lp);
//        (__builtin_expect((!(path2 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2028,"path2 >=0") : ((void )0);
        int q_stored_in_path1 = -1;
        if (path2) {
          __restore__(q_stored_in_path1,lp);
//          (__builtin_expect((!(q_stored_in_path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2034,"q_stored_in_path1 >= 0") : ((void )0);
          if (!q_stored_in_path1) {
            __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
          }
          else {
          }
        }
        else {
        }
        int path1 = -1;
        __restore__(path1,lp);
//        (__builtin_expect((!(path1 >= 0)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2041,"path1 >=0") : ((void )0);
        if (path1) {
          bs_rand_rvs(s -> airport_state::rn,lp);
          __restore__(s -> airport_state::cdelay_airport_land,lp);
          ::__restore__(s -> airport_state::delay_airport_land,lp);
          __restore__< std::deque< Aircraft  , std::allocator< Aircraft  >  >  > (( *(s -> airport_state::controller)).Controller::m_q,lp);
//assert(q_stored_in_path1 == 1);
          s -> airport_state::landing_processed--;
          ( *(s -> airport_state::controller)).Controller::m_aircraft_processed--;
          ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
        }
        else {
        }
        break; 
      }
    }
    case TAXI_IN:
{
{
        bs_rand_rvs(s -> airport_state::rn,lp);
        __restore__(s -> airport_state::max_counter,lp);
        __restore__(s -> airport_state::max_counter_aircraft_id,lp);
        ( *(s -> airport_state::counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]--;
        break; 
      }
    }
    case ARRIVAL:
{
{
        bs_rand_rvs(s -> airport_state::rn,lp);
        bs_rand_rvs(s -> airport_state::rn,lp);
        bs_rand_rvs(s -> airport_state::rn,lp);
        bs_rand_rvs(s -> airport_state::rn,lp);
        bs_rand_rvs(s -> airport_state::rn,lp);
        ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
        break; 
      }
    }
  }
}

void final(airport_state *s,tw_lp *lp)
{
//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
  total_transit_req_accepted += (s -> airport_state::transit_req_accepted);
//cout<<lp->gid<<","<<total_transit_req_accepted<<endl;
  total_transit_req_rejected += (s -> airport_state::transit_req_rejected);
  total_transit_processed += (s -> airport_state::transit_processed);
  total_dep_processed += (s -> airport_state::dep_processed);
  total_dep_req_accepted += (s -> airport_state::dep_req_accepted);
  total_dep_req_rejected += (s -> airport_state::dep_req_rejected);
  total_landing_processed += (s -> airport_state::landing_processed);
  total_landing_req_accepted += (s -> airport_state::landing_req_accepted);
  total_landing_req_rejected += (s -> airport_state::landing_req_rejected);
  max_queue_size_airport = (((s -> airport_state::max_queue_size_airport) > max_queue_size_airport)?(s -> airport_state::max_queue_size_airport) : max_queue_size_airport);
  max_queue_size_region = (((s -> airport_state::max_queue_size_region) > max_queue_size_region)?(s -> airport_state::max_queue_size_region) : max_queue_size_region);
  total_cdelay_airport_dep += (s -> airport_state::cdelay_airport_dep);
  total_delay_airport_dep += (s -> airport_state::delay_airport_dep);
  total_cdelay_airport_land += (s -> airport_state::cdelay_airport_land);
  total_delay_airport_land += (s -> airport_state::delay_airport_land);
  total_cdelay_region += (s -> airport_state::cdelay_region);
  total_delay_region += (s -> airport_state::delay_region);
//cout<<lp->gid<<","<<total_cdelay<<endl;
}
/*
   Parallel Running
 */
tw_lptype airport_lps[] = {{((init_f )init), ((event_f )event_handler_fw), ((revent_f )event_handler_rv), ((final_f )final), ((map_f )mapping_to_pe), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_STIME), ("mean"), ("mean flight time for planes"), ((&mean_flight_time))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, 
//TWOPT_UINT("loopsize", loop_size, "padding loop size"),
{(TWOPTTYPE_UINT), ("prun"), ("parallel run"), ((&p_run))}, {((tw_opttype )0), (0L), (0L), (0L)}};


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
  int nlp_per_kp;
  nlp_per_kp = (ceil((((double )g_tw_nlp) / ((double )g_tw_nkp))));
  (( *(&std::cout)<<"nlp_per_kp") << nlp_per_kp) << std::endl< char  , std::char_traits< char  >  > ;
  int local_lp_count = 0;
  for (int lpid = 0; lpid < NUMBER_OF_LP; lpid++) {
    if (g_tw_mynode == mapping_to_pe(lpid)) {
      kpid = (local_lp_count / nlp_per_kp);
//cout<<local_lp_count<<",="<<kpid<<endl;
// MUST COME AFTER!! DO NOT PRE-INCREMENT ELSE KPID is WRONG!!
      local_lp_count++;
      if (kpid >= g_tw_nkp) {
        tw_error("/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2787,"Attempting to mapping a KPid (%llu) for Global LPid %llu that is beyond g_tw_nkp (%llu)\n",kpid,lpid,g_tw_nkp);
      }
      else {
      }
      tw_lp_onpe((mapping_to_local_index(lpid)),g_tw_pe[0],lpid);
//printf("tw_lp_onpe(%d, %d, %d)\n", mapping_to_local_index(lpid), g_tw_mynode, lpid);
      if (g_tw_kp[kpid] == 0L) {
        tw_kp_onpe(kpid,g_tw_pe[0]);
//printf("tw_kp_onpe(%d, %d)\n", kpid, g_tw_mynode);
      }
      else {
      }
      tw_lp_onkp(g_tw_lp[mapping_to_local_index(lpid)],g_tw_kp[kpid]);
//printf("tw_kp_onpe(%d, %d)\n", mapping_to_local_index(lpid), kpid);
      tw_lp_settype((mapping_to_local_index(lpid)),(airport_lps + 0));
    }
    else {
    }
  }
}

int main(int argc,char **argv,char **env)
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
  tw_init(&argc,&argv);
  nlp_per_pe /= ((tw_nnodes()) * g_tw_npe);
  if (tw_nnodes() != 1) {
    g_tw_mapping = CUSTOM;
    g_tw_custom_initial_mapping = air_traffic_mapping;
    g_tw_custom_lp_global_to_local_map = mapping_to_lp;
  }
  else {
  }
  g_tw_events_per_pe = (((planes_per_airport * nlp_per_pe) / g_tw_npe) + opt_mem);
  tw_define_lps(nlp_per_pe,(sizeof(air_traffic_message )),0);
  graph = (::new Graph (20));
  graph ->  create_graph (GRAPH_CSV_FILE_PATH);
//graph->print_adjmatrix();
  if (tw_nnodes() == 1) {
    for (i = 0; i < g_tw_nlp; i++) {
      tw_lp_settype(i,(airport_lps + 0));
    }
  }
  else {
  }
//init map to count how many events are executed by each lp
//key lpid, value map<event type, counter>
  if (0) {
    for (i = 0; i < NUMBER_OF_LP; i++) {
      counters. insert ((std::make_pair< int  , std::map< int  , counter_container  , std::less< int  >  , std::allocator< std::pair< const int  , counter_container  >  >  >  > (i,std::map< int ,counter_container ,std::less< int  > ,std::allocator< std::pair< const int  , counter_container  >  > > ::map())));
      for (int j = 0; i < ARRIVAL + 1; i++) {
        counter_container c = {(0), (0)};
        counters[i]. insert ((std::make_pair< int  , counter_container  > (j,c)));
      }
    }
  }
  else {
  }
//printf("loop size %d \n", loop_size);
  tw_run();
  if ((tw_ismaster())) {
    printf("\nAir Traffic Model Statistics:\n");
    printf("\t%-50s %11lld\n","Number of LPs (Airport, Region Controller)",((nlp_per_pe * g_tw_npe) * (tw_nnodes())));
    printf("\t%-50s %11lld\n","Number of planes",(((planes_per_airport * nlp_per_pe) * g_tw_npe) * (tw_nnodes())));
    (( *(&std::cout)<<"\tTotal Transit Req Accepted : ") << total_transit_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Transit Req Rejected : ") << total_transit_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Transit Processed : ") << total_transit_processed) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Departure Req Accepted : ") << total_dep_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Departure Req Rejected : ") << total_dep_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Departure Processed : ") << total_dep_processed) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Landing Req Accepted : ") << total_landing_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Landing Req Rejected : ") << total_landing_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Landing Processed : ") << total_landing_processed) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport Dep cdelay : ") << (((double )total_cdelay_airport_dep) / total_dep_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport Dep delay : ") << (total_delay_airport_dep / total_dep_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport Land cdelay : ") << (((double )total_cdelay_airport_land) / total_landing_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport Land delay : ") << (total_delay_airport_land / total_landing_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Region cdelay : ") << (((double )total_cdelay_region) / total_transit_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Region delay : ") << (total_delay_region / total_transit_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    ( *(&std::cout)<<"") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tMax Airport Queue Size : ") << max_queue_size_airport) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tMax Region Queue Size : ") << max_queue_size_region) << std::endl< char  , std::char_traits< char  >  > ;
  }
  else {
  }
  tw_end();
/********************
	  Optional Analyze Tool
	 *********************/
//cout<<path_cal<<endl;
  if (0) {
    write_map();
  }
  else {
  }
  return 0;
}

int increase_counter(int lpid,int event_type)
{
  counters[lpid][event_type].counter_container::total_event_count++;
  counters[lpid][event_type].counter_container::net_event_count++;
}

int decrease_counter(int lpid,int event_type)
{
  counters[lpid][event_type].counter_container::net_event_count--;
}

void write_map()
{
  FILE *fp;
  std::stringstream sstm;
  std::string name("lpmap");
  int mynode = g_tw_mynode;
  (sstm << tw_nnodes()<<"_"<<name<<"_") << mynode<<".txt";
  std::string file_name = sstm. str ();
  std::ofstream file(file_name. c_str ());
  if (file. is_open ()) {
    std::map< int ,inner_map ,std::less< int  > ,std::allocator< std::pair< const int  , inner_map  >  > > ::iterator it;
    std::map< int ,counter_container ,std::less< int  > ,std::allocator< std::pair< const int  , counter_container  >  > > ::iterator inner_it;
    for (it = (counters. begin ()); it != counters. end (); it ++ ) {
      for (inner_it = (( * it).std::pair< const int ,inner_map > ::second. begin ()); inner_it != ( * it).std::pair< const int ,inner_map > ::second. end (); inner_it ++ ) {
        int event_type = ( * inner_it).std::pair< const int ,counter_container > ::first;
        std::string event_name;
        if (event_type == 0) {
          event_name = "DEP_REQ";
        }
        else {
          if (event_type == 1) {
            event_name = "DEP_DELAY";
          }
          else {
            if (event_type == 2) {
              event_name = "TAXI_OUT";
            }
            else {
              if (event_type == 3) {
                event_name = "TAKE_OFF";
              }
              else {
                if (event_type == 4) {
                  event_name = "TRANSIT_REQ";
                }
                else {
                  if (event_type == 5) {
                    event_name = "ON_THE_AIR";
                  }
                  else {
                    if (event_type == 6) {
                      event_name = "TRANSIT_DELAY";
                    }
                    else {
                      if (event_type == 7) {
                        event_name = "LANDING_REQ";
                      }
                      else {
                        if (event_type == 8) {
                          event_name = "LANDING_DELAY";
                        }
                        else {
                          if (event_type == 9) {
                            event_name = "LANDING";
                          }
                          else {
                            if (event_type == 10) {
                              event_name = "TAXI_IN";
                            }
                            else {
                              event_name = "ARRIVAL";
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        std::string lp_name;
        if (( * it).std::pair< const int ,inner_map > ::first < NUMBER_OF_REGION_CONTROLLER) {
          lp_name = "RC";
        }
        else {
          lp_name = "Airport";
        }
        (((file << mynode<<","<<lp_name<<",") << ( * it).std::pair< const int ,inner_map > ::first<<","<<event_name<<",") << ( * inner_it).std::pair< const int ,counter_container > ::second.counter_container::total_event_count<<",") << ( * inner_it).std::pair< const int ,counter_container > ::second.counter_container::net_event_count<<"\n";
      }
    }
  }
  else {
    ( *(&std::cout)<<"write_map, couldn\'t open a file") << std::endl< char  , std::char_traits< char  >  > ;
  }
}

int get_region(int airport)
{
  if ((airport >= 20) && (airport <= 38)) {
    return 0;;
  }
  else {
    if ((airport >= 39) && (airport <= 53)) {
      return 1;;
    }
    else {
      if ((airport >= 54) && (airport <= 73)) {
        return 3;;
      }
      else {
        if ((airport >= 74) && (airport <= 90)) {
          return 4;;
        }
        else {
          if ((airport >= 91) && (airport <= 124)) {
            return 6;;
          }
          else {
            if ((airport >= 125) && (airport <= 143)) {
              return 2;;
            }
            else {
              if ((airport >= 144) && (airport <= 148)) {
                return 5;;
              }
              else {
                if ((airport >= 149) && (airport <= 166)) {
                  return 7;;
                }
                else {
                  if ((airport >= 167) && (airport <= 180)) {
                    return 8;;
                  }
                  else {
                    if ((airport >= 181) && (airport <= 201)) {
                      return 9;;
                    }
                    else {
                      if ((airport >= 202) && (airport <= 214)) {
                        return 10;;
                      }
                      else {
                        if ((airport >= 215) && (airport <= 231)) {
                          return 12;;
                        }
                        else {
                          if ((airport >= 232) && (airport <= 256)) {
                            return 15;;
                          }
                          else {
                            if ((airport >= 257) && (airport <= 264)) {
                              return 16;;
                            }
                            else {
                              if ((airport >= 265) && (airport <= 280)) {
                                return 17;;
                              }
                              else {
                                if ((airport >= 281) && (airport <= 292)) {
                                  return 11;;
                                }
                                else {
                                  if ((airport >= 293) && (airport <= 303)) {
                                    return 13;;
                                  }
                                  else {
                                    if ((airport >= 304) && (airport <= 318)) {
                                      return 14;;
                                    }
                                    else {
                                      if ((airport >= 319) && (airport <= 337)) {
                                        return 18;;
                                      }
                                      else {
                                        if ((airport >= 338) && (airport <= 351)) {
                                          return 19;;
                                        }
                                        else {
                                          class std::basic_ostream< char  , std::char_traits< char  >  > *__temp8__;
                                          __temp8__ = &( *(&std::cout)<<"airport region failed");
                                          ( *__temp8__) << std::endl< char  , std::char_traits< char  >  > ;;
                                          bool __temp9__;
                                          __temp9__ = !false;
                                          if ((__builtin_expect(__temp9__,0))) {
//                                            __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",3099,"false");;
                                          }
                                          else {
                                            (void )0;;
                                          };;
                                        };
                                      };
                                    };
                                  };
                                };
                              };
                            };
                          };
                        };
                      };
                    };
                  };
                };
              };
            };
          };
        };
      };
    };
  };
}
