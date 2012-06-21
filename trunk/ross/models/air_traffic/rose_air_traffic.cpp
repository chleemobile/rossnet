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
#include "rctypes.h" 
int get_region(int airport);
int mapping_to_local_index(int lpid);
int increase_counter(int lipd,int event_type);
int decrease_counter(int lipd,int event_type);
void write_map();

tw_peid mapping_to_pe(tw_lpid gid)
{
  if (tw_nnodes() == 1) {
    return (gid / nlp_per_pe);
  }
  else {
    if (tw_nnodes() == 2) {
      if ((gid < 10) || ((gid >= 20) && (gid <= 183))) {
//cout<<gid<<" return 0"<<endl;
        return 0;
      }
      else {
//cout<<gid<<" return 1"<<endl;
        return 1;
      }
    }
    else {
      if (tw_nnodes() == 4) {
        if ((((((gid == 0) || (gid == 1)) || (gid == 3)) || (gid == 4)) || (gid == 6)) || ((gid >= 20) && (gid <= 101))) {
          return 0;
        }
        else {
          if ((((((gid == 2) || (gid == 5)) || (gid == 7)) || (gid == 8)) || (gid == 9)) || ((gid >= 102) && (gid <= 183))) {
            return 1;
          }
          else {
            if ((((((gid == 10) || (gid == 12)) || (gid == 15)) || (gid == 16)) || (gid == 17)) || ((gid >= 184) && (gid <= 265))) {
              return 2;
            }
            else {
              return 3;
            }
          }
        }
      }
      else {
        ( *(&std::cout)<<"support upto 4 cores") << std::endl< char  , std::char_traits< char  >  > ;
        (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",71,"false") : ((void )0);
      }
    }
  }
}

void init(airport_state *s,tw_lp *lp)
{
  static int init_seed = (lp -> tw_lp::gid);
  struct BSStack *stack = ::new BSStack ;
  lp -> tw_lp::stack_pointer = stack;
  int i;
  tw_event *e;
  air_traffic_message *m;
  tw_stime ts;
  s -> airport_state::rn = (lp -> tw_lp::gid);
  s -> airport_state::incoming_queue = (::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > );
  (__builtin_expect((!( *(s -> airport_state::incoming_queue)). empty ()),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",89,"s->incoming_queue->empty()") : ((void )0);
  s -> airport_state::aircraft_counter = (::new class std::vector< int  , std::allocator< int  >  > (656));
  s -> airport_state::max_queue_size_airport = 0;
  s -> airport_state::max_queue_size_region = 0;
  s -> airport_state::delay_airport_dep = 0;
  s -> airport_state::delay_airport_land = 0;
  s -> airport_state::cdelay_airport_dep = 0;
  s -> airport_state::cdelay_airport_dep = 0;
  s -> airport_state::delay_region = 0;
  s -> airport_state::cdelay_region = 0;
  if ((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER) {
    if (((((((lp -> tw_lp::gid) == 4) || ((lp -> tw_lp::gid) == 5)) || ((lp -> tw_lp::gid) == 8)) || ((lp -> tw_lp::gid) == 11)) || ((lp -> tw_lp::gid) == 13)) || ((lp -> tw_lp::gid) == 16)) {
      s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;
      s -> airport_state::controller = (::new RegionController (AIRCRAFT_CAPACITY_OF_SMALL_REGION));
    }
    else {
      if ((((((((((lp -> tw_lp::gid) == 0) || ((lp -> tw_lp::gid) == 1)) || ((lp -> tw_lp::gid) == 3)) || ((lp -> tw_lp::gid) == 7)) || ((lp -> tw_lp::gid) == 10)) || ((lp -> tw_lp::gid) == 12)) || ((lp -> tw_lp::gid) == 14)) || ((lp -> tw_lp::gid) == 18)) || ((lp -> tw_lp::gid) == 19)) {
        s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;
        s -> airport_state::controller = (::new RegionController (AIRCRAFT_CAPACITY_OF_MEDIUM_REGION));
      }
      else {
        s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
        s -> airport_state::controller = (::new RegionController (AIRCRAFT_CAPACITY_OF_LARGE_REGION));
      }
    }
    s -> airport_state::airplane_in_region = 0;
    s -> airport_state::transit_req_accepted = 0;
    s -> airport_state::transit_req_rejected = 0;
    s -> airport_state::transit_processed = 0;
  }
  else {
    if (((((((((((((((((((lp -> tw_lp::gid) == 20) || ((lp -> tw_lp::gid) == 39)) || ((lp -> tw_lp::gid) == 73)) || ((lp -> tw_lp::gid) == 93)) || ((lp -> tw_lp::gid) == 115)) || ((lp -> tw_lp::gid) == 149)) || ((lp -> tw_lp::gid) == 167)) || ((lp -> tw_lp::gid) == 181)) || ((lp -> tw_lp::gid) == 227)) || ((lp -> tw_lp::gid) == 244)) || ((lp -> tw_lp::gid) == 319)) || (((lp -> tw_lp::gid) >= 54) && ((lp -> tw_lp::gid) <= 56))) || (((lp -> tw_lp::gid) >= 202) && ((lp -> tw_lp::gid) <= 203))) || (((lp -> tw_lp::gid) >= 0xff) && ((lp -> tw_lp::gid) <= 256))) || (((lp -> tw_lp::gid) >= 270) && ((lp -> tw_lp::gid) <= 272))) || (((lp -> tw_lp::gid) >= 295) && ((lp -> tw_lp::gid) <= 296))) || (((lp -> tw_lp::gid) >= 303) && ((lp -> tw_lp::gid) <= 305))) || (((lp -> tw_lp::gid) >= 338) && ((lp -> tw_lp::gid) <= 340))) {
      s -> airport_state::max_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
    }
    else {
      if ((((((((((((((((((lp -> tw_lp::gid) == 21) || ((lp -> tw_lp::gid) == 39)) || ((lp -> tw_lp::gid) == 110)) || ((lp -> tw_lp::gid) == 116)) || ((lp -> tw_lp::gid) == 168)) || ((lp -> tw_lp::gid) == 204)) || ((lp -> tw_lp::gid) == 320)) || (((lp -> tw_lp::gid) >= 40) && ((lp -> tw_lp::gid) <= 43))) || (((lp -> tw_lp::gid) >= 57) && ((lp -> tw_lp::gid) <= 59))) || (((lp -> tw_lp::gid) >= 150) && ((lp -> tw_lp::gid) <= 152))) || (((lp -> tw_lp::gid) >= 182) && ((lp -> tw_lp::gid) <= 185))) || (((lp -> tw_lp::gid) >= 215) && ((lp -> tw_lp::gid) <= 216))) || (((lp -> tw_lp::gid) >= 228) && ((lp -> tw_lp::gid) <= 230))) || (((lp -> tw_lp::gid) >= 245) && ((lp -> tw_lp::gid) <= 246))) || (((lp -> tw_lp::gid) >= 273) && ((lp -> tw_lp::gid) <= 275))) || (((lp -> tw_lp::gid) >= 306) && ((lp -> tw_lp::gid) <= 307))) || (((lp -> tw_lp::gid) >= 341) && ((lp -> tw_lp::gid) <= 342))) {
        s -> airport_state::max_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
      }
      else {
        if (((((((((((((((((((((lp -> tw_lp::gid) == 22) || ((lp -> tw_lp::gid) == 44)) || ((lp -> tw_lp::gid) == 94)) || ((lp -> tw_lp::gid) == 343)) || (((lp -> tw_lp::gid) >= 60) && ((lp -> tw_lp::gid) <= 62))) || (((lp -> tw_lp::gid) >= 74) && ((lp -> tw_lp::gid) <= 75))) || (((lp -> tw_lp::gid) >= 111) && ((lp -> tw_lp::gid) <= 112))) || (((lp -> tw_lp::gid) >= 117) && ((lp -> tw_lp::gid) <= 120))) || (((lp -> tw_lp::gid) >= 153) && ((lp -> tw_lp::gid) <= 155))) || (((lp -> tw_lp::gid) >= 169) && ((lp -> tw_lp::gid) <= 171))) || (((lp -> tw_lp::gid) >= 186) && ((lp -> tw_lp::gid) <= 188))) || (((lp -> tw_lp::gid) >= 205) && ((lp -> tw_lp::gid) <= 207))) || (((lp -> tw_lp::gid) >= 217) && ((lp -> tw_lp::gid) <= 219))) || (((lp -> tw_lp::gid) >= 231) && ((lp -> tw_lp::gid) <= 234))) || (((lp -> tw_lp::gid) >= 247) && ((lp -> tw_lp::gid) <= 250))) || (((lp -> tw_lp::gid) >= 257) && ((lp -> tw_lp::gid) <= 261))) || (((lp -> tw_lp::gid) >= 276) && ((lp -> tw_lp::gid) <= 281))) || (((lp -> tw_lp::gid) >= 297) && ((lp -> tw_lp::gid) <= 298))) || (((lp -> tw_lp::gid) >= 308) && ((lp -> tw_lp::gid) <= 310))) || (((lp -> tw_lp::gid) >= 321) && ((lp -> tw_lp::gid) <= 327))) {
          s -> airport_state::max_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
        }
        else {
          s -> airport_state::max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
        }
      }
    }
    s -> airport_state::runway_in_use = 0;
    s -> airport_state::landing_processed = 0;
    s -> airport_state::landing_req_accepted = 0;
    s -> airport_state::landing_req_rejected = 0;
    s -> airport_state::dep_processed = 0;
    s -> airport_state::dep_req_accepted = 0;
    s -> airport_state::dep_req_rejected = 0;
    for (i = 0; i < planes_per_airport; i++) {
      int event_send_to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      class Aircraft aircraft;
      double dep_time = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      dep_time += tw_now(lp);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,(NUMBER_OF_LP - 1)));
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
      e = tw_event_new(event_send_to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP_REQ;
      m -> air_traffic_message::aircraft = (aircraft);
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
      bool __temp4__;
      __temp4__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp4__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",262,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
      }
      else {
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::dep_req_accepted++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
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
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
        bool __temp5__;
        __temp5__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp5__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",332,"old_top.m_id == new_top.m_id");
        }
        else {
          (void )0;
        }
        bool __temp6__;
        __temp6__ = !(old_size == new_size);
        if ((__builtin_expect(__temp6__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",333,"old_size == new_size");
        }
        else {
          (void )0;
        }
      }
      else {
      }
      goto LABEL11;
    }
    LABEL1:
    if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
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
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
      bool __temp7__;
      __temp7__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp7__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",402,"old_top.m_id == new_top.m_id");
      }
      else {
        (void )0;
      }
      bool __temp8__;
      __temp8__ = !(old_size == new_size);
      if ((__builtin_expect(__temp8__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",403,"old_size == new_size");
      }
      else {
        (void )0;
      }
    }
    else {
    }
    goto LABEL11;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
      ( *(s -> airport_state::aircraft_counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]++;
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAKE_OFF;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL3:
{
      s -> airport_state::runway_in_use--;
      int src_region = get_region((lp -> tw_lp::gid));
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        next_region = path. front ();
        bool __temp9__;
        __temp9__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp9__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",445,"next_region == msg->aircraft.m_dest_region");
        }
        else {
          (void )0;
        }
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
      goto LABEL11;
    }
    LABEL4:
{
      bool __temp10__;
      __temp10__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp10__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",488,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_region)) {
        s -> airport_state::max_queue_size_region = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
      }
//				if (s->airplane_in_region < s->max_capacity)
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
          s -> airport_state::airplane_in_region++;
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::incoming_queue)). pop ();
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
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
        bool __temp11__;
        __temp11__ = !(( *(s -> airport_state::incoming_queue)). size () > 0);
        if ((__builtin_expect(__temp11__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",540,"s->incoming_queue->size() > 0");
        }
        else {
          (void )0;
        }
        s -> airport_state::transit_req_rejected++;
      }
//update remaining distance, process time, delays
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
          temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
          temp.Aircraft::m_cdelay++;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
//assert(old_top.m_id == new_top.m_id);
        bool __temp12__;
        __temp12__ = !(old_size == new_size);
        if ((__builtin_expect(__temp12__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",579,"old_size == new_size");
        }
        else {
          (void )0;
        }
      }
      else {
      }
      goto LABEL11;
    }
    LABEL5:
//assert(false);
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) 
//if (s->airplane_in_region < s->max_capacity) 
{
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::transit_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
          ( *(s -> airport_state::incoming_queue)). pop ();
          s -> airport_state::airplane_in_region++;
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
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
    }
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
        temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
        temp.Aircraft::m_cdelay++;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
//assert(old_top.m_id == new_top.m_id);
      bool __temp13__;
      __temp13__ = !(old_size == new_size);
      if ((__builtin_expect(__temp13__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",662,"old_size == new_size");
      }
      else {
        (void )0;
      }
    }
    else {
    }
    goto LABEL11;
    LABEL6:
{
      bool __temp14__;
      __temp14__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp14__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",674,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        (void )0;
      }
      s -> airport_state::airplane_in_region--;
      ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
      int src_region = (lp -> tw_lp::gid);
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        next_region = path. front ();
        bool __temp15__;
        __temp15__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp15__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",693,"next_region == msg->aircraft.m_dest_region");
        }
        else {
          (void )0;
        }
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
      goto LABEL11;
    }
    LABEL7:
{
      bool __temp16__;
      __temp16__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp16__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",759,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
      }
      else {
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_req_accepted++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::delay_airport_land += (tw_now(lp) - aircraft.Aircraft::m_clock);
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
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
        bool __temp17__;
        __temp17__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp17__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",829,"old_top.m_id == new_top.m_id");
        }
        else {
          (void )0;
        }
        bool __temp18__;
        __temp18__ = !(old_size == new_size);
        if ((__builtin_expect(__temp18__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",830,"old_size == new_size");
        }
        else {
          (void )0;
        }
      }
      else {
      }
      goto LABEL11;
    }
    LABEL8:
    if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
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
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
      bool __temp19__;
      __temp19__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp19__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",891,"old_top.m_id == new_top.m_id");
      }
      else {
        (void )0;
      }
      bool __temp20__;
      __temp20__ = !(old_size == new_size);
      if ((__builtin_expect(__temp20__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",892,"old_size == new_size");
      }
      else {
        (void )0;
      }
    }
    else {
    }
    goto LABEL11;
    LABEL9:
{
      ( *(s -> airport_state::aircraft_counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]++;
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = ARRIVAL;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL10:
{
      s -> airport_state::runway_in_use--;
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      double dep_time = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      dep_time += tw_now(lp);
      int __temp21__;
      __temp21__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp21__));
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
      goto LABEL11;
    }
    LABEL11:;
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
      __num0 += 65536;
      if (__item_selector__ == DEP) {
        goto LABEL1;
      }
      else {
        __num0 += 32768;
        if (__item_selector__ == TAXI_OUT) {
          goto LABEL2;
        }
        else {
          __num0 += 16384;
          if (__item_selector__ == TAKE_OFF) {
            goto LABEL3;
          }
          else {
            __num0 += 8192;
            if (__item_selector__ == TRANSIT_REQ) {
              goto LABEL4;
            }
            else {
              __num0 += 4096;
              if (__item_selector__ == TRANSIT) {
                goto LABEL5;
              }
              else {
                __num0 += 2048;
                if (__item_selector__ == ON_THE_AIR) {
                  goto LABEL6;
                }
                else {
                  __num0 += 1024;
                  if (__item_selector__ == LANDING_REQ) {
                    goto LABEL7;
                  }
                  else {
                    __num0 += 512;
                    if (__item_selector__ == LANDING) {
                      goto LABEL8;
                    }
                    else {
                      __num0 += 256;
                      if (__item_selector__ == TAXI_IN) {
                        goto LABEL9;
                      }
                      else {
                        __num0 += 128;
                        if (__item_selector__ == ARRIVAL) {
                          goto LABEL10;
                        }
                        else {
                          __num0 += 64;
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
      bool __temp4__;
      __temp4__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp4__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",262,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
      }
      else {
        __num0 += 32;
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      __store__( *s -> airport_state::incoming_queue,lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        __store__< int  > (s -> airport_state::max_queue_size_airport,lp);
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
        __num0 += 16;
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::dep_req_accepted++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        __store__(s -> airport_state::delay_airport_dep,lp);
        s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
        __store__< int  > (s -> airport_state::cdelay_airport_dep,lp);
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 8;
        s -> airport_state::dep_req_rejected++;
      }
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
        bool __temp5__;
        __temp5__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp5__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",332,"old_top.m_id == new_top.m_id");
        }
        else {
          __num0 += 2;
          (void )0;
        }
        bool __temp6__;
        __temp6__ = !(old_size == new_size);
        if ((__builtin_expect(__temp6__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",333,"old_size == new_size");
        }
        else {
          __num0 += 1;
          (void )0;
        }
      }
      else {
        __num0 += 4;
      }
      goto LABEL11;
    }
    LABEL1:
    if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        __store__( *s -> airport_state::incoming_queue,lp);
        ( *(s -> airport_state::incoming_queue)). pop ();
        __store__(s -> airport_state::delay_airport_dep,lp);
        s -> airport_state::delay_airport_dep += (tw_now(lp) - aircraft.Aircraft::m_clock);
        __store__< int  > (s -> airport_state::cdelay_airport_dep,lp);
        s -> airport_state::cdelay_airport_dep += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_OUT;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 8;
      }
    }
    else {
      __num0 += 16;
    }
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      __store__( *s -> airport_state::incoming_queue,lp);
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
      bool __temp7__;
      __temp7__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp7__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",402,"old_top.m_id == new_top.m_id");
      }
      else {
        __num0 += 2;
        (void )0;
      }
      bool __temp8__;
      __temp8__ = !(old_size == new_size);
      if ((__builtin_expect(__temp8__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",403,"old_size == new_size");
      }
      else {
        __num0 += 1;
        (void )0;
      }
    }
    else {
      __num0 += 4;
    }
    goto LABEL11;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
      __store__( *s -> airport_state::aircraft_counter,lp);
      ( *(s -> airport_state::aircraft_counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]++;
      int to = (lp -> tw_lp::gid);
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAKE_OFF;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL3:
{
      s -> airport_state::runway_in_use--;
      int src_region = get_region((lp -> tw_lp::gid));
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        __num0 += 4;
        next_region = path. front ();
        bool __temp9__;
        __temp9__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp9__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",445,"next_region == msg->aircraft.m_dest_region");
        }
        else {
          __num0 += 2;
          (void )0;
        }
      }
      float distance = (graph -> Graph::adjMatrix)[src_region][next_region];
      if (distance < 0) {
        distance = 0;
      }
      else {
        __num0 += 1;
      }
      int to = next_region;
      __store__< int  > (s -> airport_state::rn,lp);
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
      goto LABEL11;
    }
    LABEL4:
{
      bool __temp10__;
      __temp10__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp10__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",488,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        __num0 += 32;
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      __store__( *s -> airport_state::incoming_queue,lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_region)) {
        __store__< int  > (s -> airport_state::max_queue_size_region,lp);
        s -> airport_state::max_queue_size_region = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
        __num0 += 16;
      }
//				if (s->airplane_in_region < s->max_capacity)
      if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) {
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          __store__< int  > (s -> airport_state::controller -> Controller::m_current_capacity,lp);
          ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
          s -> airport_state::airplane_in_region++;
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::transit_processed++;
          ( *(s -> airport_state::incoming_queue)). pop ();
          __store__(s -> airport_state::delay_region,lp);
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
          __store__< int  > (s -> airport_state::cdelay_region,lp);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          __store__< int  > (s -> airport_state::rn,lp);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
          __num0 += 4;
        }
      }
      else {
        __num0 += 8;
        bool __temp11__;
        __temp11__ = !(( *(s -> airport_state::incoming_queue)). size () > 0);
        if ((__builtin_expect(__temp11__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",540,"s->incoming_queue->size() > 0");
        }
        else {
          __num0 += 4;
          (void )0;
        }
        s -> airport_state::transit_req_rejected++;
      }
//update remaining distance, process time, delays
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
          temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
          temp.Aircraft::m_cdelay++;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
//assert(old_top.m_id == new_top.m_id);
        bool __temp12__;
        __temp12__ = !(old_size == new_size);
        if ((__builtin_expect(__temp12__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",579,"old_size == new_size");
        }
        else {
          __num0 += 1;
          (void )0;
        }
      }
      else {
        __num0 += 2;
      }
      goto LABEL11;
    }
    LABEL5:
//assert(false);
    if (( *(s -> airport_state::controller)).Controller::m_current_capacity < ( *(s -> airport_state::controller)).Controller::m_max_capacity) 
//if (s->airplane_in_region < s->max_capacity) 
{
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::transit_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          __store__< int  > (s -> airport_state::controller -> Controller::m_current_capacity,lp);
          ( *(s -> airport_state::controller)).Controller::m_current_capacity++;
          __store__( *s -> airport_state::incoming_queue,lp);
          ( *(s -> airport_state::incoming_queue)). pop ();
          s -> airport_state::airplane_in_region++;
          __store__(s -> airport_state::delay_region,lp);
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
          __store__< int  > (s -> airport_state::cdelay_region,lp);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
          aircraft.Aircraft::m_clock = 0;
          aircraft.Aircraft::m_cdelay = 0;
          aircraft.Aircraft::m_delay = 0;
          int to = (lp -> tw_lp::gid);
          __store__< int  > (s -> airport_state::rn,lp);
          ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
          e = tw_event_new(to,ts,lp);
          m = ((air_traffic_message *)(tw_event_data(e)));
          m -> air_traffic_message::type = ON_THE_AIR;
          m -> air_traffic_message::aircraft = (aircraft);
          m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
          tw_event_send(e);
        }
        else {
          __num0 += 4;
        }
      }
      else {
        __num0 += 8;
      }
    }
    else {
      __num0 += 16;
    }
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      __store__( *s -> airport_state::incoming_queue,lp);
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
        temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
        temp.Aircraft::m_cdelay++;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
//assert(old_top.m_id == new_top.m_id);
      bool __temp13__;
      __temp13__ = !(old_size == new_size);
      if ((__builtin_expect(__temp13__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",662,"old_size == new_size");
      }
      else {
        __num0 += 1;
        (void )0;
      }
    }
    else {
      __num0 += 2;
    }
    goto LABEL11;
    LABEL6:
{
      bool __temp14__;
      __temp14__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp14__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",674,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        __num0 += 16;
        (void )0;
      }
      s -> airport_state::airplane_in_region--;
      __store__< int  > (s -> airport_state::controller -> Controller::m_current_capacity,lp);
      ( *(s -> airport_state::controller)).Controller::m_current_capacity--;
      int src_region = (lp -> tw_lp::gid);
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        __num0 += 8;
        next_region = path. front ();
        bool __temp15__;
        __temp15__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp15__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",693,"next_region == msg->aircraft.m_dest_region");
        }
        else {
          __num0 += 4;
          (void )0;
        }
      }
      if (next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region) {
        int to = msg -> air_traffic_message::aircraft.Aircraft::m_dest_airport;
        __store__< int  > (s -> airport_state::rn,lp);
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
        __store__< int  > (s -> airport_state::rn,lp);
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
      goto LABEL11;
    }
    LABEL7:
{
      bool __temp16__;
      __temp16__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp16__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",759,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
      }
      else {
        __num0 += 32;
        (void )0;
      }
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      __store__( *s -> airport_state::incoming_queue,lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        __store__< int  > (s -> airport_state::max_queue_size_airport,lp);
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
        __num0 += 16;
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_req_accepted++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        __store__(s -> airport_state::delay_airport_land,lp);
        s -> airport_state::delay_airport_land += (tw_now(lp) - aircraft.Aircraft::m_clock);
        __store__< int  > (s -> airport_state::cdelay_airport_land,lp);
        s -> airport_state::cdelay_airport_land += aircraft.Aircraft::m_cdelay;
        aircraft.Aircraft::m_clock = 0;
        aircraft.Aircraft::m_cdelay = 0;
        aircraft.Aircraft::m_delay = 0;
        int to = (lp -> tw_lp::gid);
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        tw_event_send(e);
      }
      else {
        __num0 += 8;
        s -> airport_state::landing_req_rejected++;
      }
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
        class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
        int old_size = (( *(s -> airport_state::incoming_queue)). size ());
        while(!( *(s -> airport_state::incoming_queue)). empty ()){
          class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
          ( *(s -> airport_state::incoming_queue)). pop ();
          temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
          temp_q ->  push (temp);
        }
        while(!temp_q ->  empty ()){
          class Aircraft temp(temp_q ->  top ());
          temp_q ->  pop ();
          ( *(s -> airport_state::incoming_queue)). push (temp);
        }
        class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
        int new_size = (( *(s -> airport_state::incoming_queue)). size ());
        delete temp_q;
        bool __temp17__;
        __temp17__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp17__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",829,"old_top.m_id == new_top.m_id");
        }
        else {
          __num0 += 2;
          (void )0;
        }
        bool __temp18__;
        __temp18__ = !(old_size == new_size);
        if ((__builtin_expect(__temp18__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",830,"old_size == new_size");
        }
        else {
          __num0 += 1;
          (void )0;
        }
      }
      else {
        __num0 += 4;
      }
      goto LABEL11;
    }
    LABEL8:
    if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
      if (( *(s -> airport_state::incoming_queue)). size () > 0) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        __store__( *s -> airport_state::incoming_queue,lp);
        ( *(s -> airport_state::incoming_queue)). pop ();
        int to = (lp -> tw_lp::gid);
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = TAXI_IN;
        m -> air_traffic_message::aircraft = (aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      else {
        __num0 += 8;
      }
    }
    else {
      __num0 += 16;
    }
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      __store__( *s -> airport_state::incoming_queue,lp);
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
        temp_q ->  push (temp);
      }
      while(!temp_q ->  empty ()){
        class Aircraft temp(temp_q ->  top ());
        temp_q ->  pop ();
        ( *(s -> airport_state::incoming_queue)). push (temp);
      }
      class Aircraft new_top(( *(s -> airport_state::incoming_queue)). top ());
      int new_size = (( *(s -> airport_state::incoming_queue)). size ());
      delete temp_q;
      bool __temp19__;
      __temp19__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp19__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",891,"old_top.m_id == new_top.m_id");
      }
      else {
        __num0 += 2;
        (void )0;
      }
      bool __temp20__;
      __temp20__ = !(old_size == new_size);
      if ((__builtin_expect(__temp20__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",892,"old_size == new_size");
      }
      else {
        __num0 += 1;
        (void )0;
      }
    }
    else {
      __num0 += 4;
    }
    goto LABEL11;
    LABEL9:
{
      __store__( *s -> airport_state::aircraft_counter,lp);
      ( *(s -> airport_state::aircraft_counter))[msg -> air_traffic_message::aircraft.Aircraft::m_id]++;
      int to = (lp -> tw_lp::gid);
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = ARRIVAL;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL10:
{
      s -> airport_state::runway_in_use--;
      int to = (lp -> tw_lp::gid);
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      double dep_time = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
      dep_time += tw_now(lp);
      int __temp21__;
      __temp21__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp21__));
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
      goto LABEL11;
    }
    LABEL11:;
  }
  __store__< int  > (__num0,lp);
}

void event_handler_reverse(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
  int __num0;
  __restore__(__num0,lp);
  if ((__num0 & 131008) == 130944) {
    __restore__(s -> airport_state::rn,lp);
    ++s -> airport_state::runway_in_use;
  }
  else {
    if ((__num0 & 130944) == 130816) {
      __restore__(s -> airport_state::rn,lp);
      __restore__( *s -> airport_state::aircraft_counter,lp);
    }
    else {
      if ((__num0 & 122880) == 114688) {
        __restore__(s -> airport_state::rn,lp);
        ++s -> airport_state::runway_in_use;
      }
      else {
        if ((__num0 & 129024) == 126976) {
          if ((__num0 & 129026) == 126976) {
            __restore__( *s -> airport_state::incoming_queue,lp);
          }
          else {
          }
          if ((__num0 & 129040) == 126976) {
            if ((__num0 & 129048) == 126976) {
              if ((__num0 & 129052) == 126980) {
              }
              else {
                __restore__(s -> airport_state::rn,lp);
                __restore__(s -> airport_state::cdelay_region,lp);
                __restore__(s -> airport_state::delay_region,lp);
                --s -> airport_state::airplane_in_region;
                __restore__( *s -> airport_state::incoming_queue,lp);
                __restore__(s -> airport_state::controller -> Controller::m_current_capacity,lp);
              }
              --s -> airport_state::transit_processed;
            }
            else {
            }
          }
          else {
          }
        }
        else {
          if ((__num0 & 130816) == 130560) {
            if ((__num0 & 130820) == 130560) {
              __restore__( *s -> airport_state::incoming_queue,lp);
            }
            else {
            }
            if ((__num0 & 130832) == 130560) {
              if ((__num0 & 130840) == 130568) {
              }
              else {
                __restore__(s -> airport_state::rn,lp);
                __restore__( *s -> airport_state::incoming_queue,lp);
                --s -> airport_state::landing_processed;
                --s -> airport_state::runway_in_use;
              }
            }
            else {
            }
          }
          else {
            if ((__num0 & 98304) == 65536) {
              if ((__num0 & 98308) == 65536) {
                __restore__( *s -> airport_state::incoming_queue,lp);
              }
              else {
              }
              if ((__num0 & 98320) == 65536) {
                if ((__num0 & 98328) == 65544) {
                }
                else {
                  __restore__(s -> airport_state::rn,lp);
                  __restore__(s -> airport_state::cdelay_airport_dep,lp);
                  __restore__(s -> airport_state::delay_airport_dep,lp);
                  __restore__( *s -> airport_state::incoming_queue,lp);
                  --s -> airport_state::dep_processed;
                  --s -> airport_state::runway_in_use;
                }
              }
              else {
              }
            }
            else {
              if ((__num0 & 130048) == 129024) {
                if ((__num0 & 130050) == 129024) {
                  __restore__(s -> airport_state::rn,lp);
                }
                else {
                  __restore__(s -> airport_state::rn,lp);
                }
                __restore__(s -> airport_state::controller -> Controller::m_current_capacity,lp);
                ++s -> airport_state::airplane_in_region;
              }
              else {
                if ((__num0 & 130560) == 130048) {
                  if ((__num0 & 130568) == 130056) {
                    --s -> airport_state::landing_req_rejected;
                  }
                  else {
                    __restore__(s -> airport_state::rn,lp);
                    __restore__(s -> airport_state::cdelay_airport_land,lp);
                    __restore__(s -> airport_state::delay_airport_land,lp);
                    --s -> airport_state::landing_processed;
                    --s -> airport_state::landing_req_accepted;
                    --s -> airport_state::runway_in_use;
                  }
                  if ((__num0 & 130576) == 130064) {
                  }
                  else {
                    __restore__(s -> airport_state::max_queue_size_airport,lp);
                  }
                  __restore__( *s -> airport_state::incoming_queue,lp);
                }
                else {
                  if ((__num0 & 126976) == 122880) {
                    if ((__num0 & 126984) == 122880) {
                      if ((__num0 & 126988) == 122884) {
                      }
                      else {
                        __restore__(s -> airport_state::rn,lp);
                        __restore__(s -> airport_state::cdelay_region,lp);
                        __restore__(s -> airport_state::delay_region,lp);
                        --s -> airport_state::transit_processed;
                        --s -> airport_state::transit_req_accepted;
                        --s -> airport_state::airplane_in_region;
                        __restore__(s -> airport_state::controller -> Controller::m_current_capacity,lp);
                      }
                    }
                    else {
                      --s -> airport_state::transit_req_rejected;
                    }
                    if ((__num0 & 126992) == 122896) {
                    }
                    else {
                      __restore__(s -> airport_state::max_queue_size_region,lp);
                    }
                    __restore__( *s -> airport_state::incoming_queue,lp);
                  }
                  else {
                    if ((__num0 & 65536) == 0 || (__num0 & 131008) == 131008) {
                      if ((__num0 & 65544) == 8 || (__num0 & 131016) == 131016) {
                        --s -> airport_state::dep_req_rejected;
                      }
                      else {
                        __restore__(s -> airport_state::rn,lp);
                        __restore__(s -> airport_state::cdelay_airport_dep,lp);
                        __restore__(s -> airport_state::delay_airport_dep,lp);
                        --s -> airport_state::dep_processed;
                        --s -> airport_state::dep_req_accepted;
                        --s -> airport_state::runway_in_use;
                      }
                      if ((__num0 & 65552) == 16 || (__num0 & 131024) == 131024) {
                      }
                      else {
                        __restore__(s -> airport_state::max_queue_size_airport,lp);
                      }
                      __restore__( *s -> airport_state::incoming_queue,lp);
                    }
                    else {
                      __restore__(s -> airport_state::rn,lp);
                      __restore__( *s -> airport_state::aircraft_counter,lp);
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

void final(airport_state *s,tw_lp *lp)
{
//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
  total_transit_req_accepted += (s -> airport_state::transit_req_accepted);
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
tw_lptype airport_lps[] = {{((init_f )init), ((event_f )event_handler_forward), ((revent_f )event_handler_reverse), ((final_f )final), ((map_f )mapping_to_pe), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_STIME), ("mean"), ("mean flight time for planes"), ((&mean_flight_time))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, {(TWOPTTYPE_UINT), ("loopsize"), ("padding loop size"), ((&loop_size))}, {(TWOPTTYPE_UINT), ("prun"), ("parallel run"), ((&p_run))}, {((tw_opttype )0), (0L), (0L), (0L)}};

tw_lp *mapping_to_lp(tw_lpid lpid)
{
  int ret = lpid;
  if (tw_nnodes() == 2) {
    if (g_tw_mynode == 0) {
      if (lpid >= 20) {
        (__builtin_expect((!(lpid < 184)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1043,"lpid < 184") : ((void )0);
        ret = (ret - 10);
      }
      else {
      }
    }
    else {
      if ((lpid >= 10) && (lpid <= 19)) {
        ret = (ret + 164);
      }
      else {
      }
      ret = (ret - 174);
    }
  }
  else {
    if (tw_nnodes() == 4) {
      if (g_tw_mynode == 0) {
        if ((lpid == 3) || (lpid == 4)) {
          ret = (ret - 1);
        }
        else {
        }
        if (lpid == 6) {
          ret = (ret - 2);
        }
        else {
        }
        if ((lpid >= 20) && (lpid <= 101)) {
          ret = (ret - 15);
        }
        else {
        }
      }
      else {
        if (g_tw_mynode == 1) {
          if (lpid == 2) {
            ret = (ret + 85);
          }
          else {
          }
          if (lpid == 5) {
            ret = (ret + 83);
          }
          else {
          }
          if ((lpid >= 7) && (lpid <= 9)) {
            ret = (ret + 82);
          }
          else {
          }
          if ((lpid >= 102) && (lpid <= 183)) {
            ret = (ret - 10);
          }
          else {
          }
          ret = (ret - 87);
        }
        else {
          if (g_tw_mynode == 2) {
            if (lpid == 10) {
              ret = (ret + 164);
            }
            else {
            }
            if (lpid == 12) {
              ret = (ret + 163);
            }
            else {
            }
            if ((lpid >= 15) && (lpid <= 17)) {
              ret = (ret + 161);
            }
            else {
            }
            if ((lpid >= 184) && (lpid <= 265)) {
              ret = (ret - 5);
            }
            else {
            }
            ret = (ret - 174);
          }
          else {
            if (lpid == 11) {
              ret = (ret + 250);
            }
            else {
            }
            if ((lpid == 13) || (lpid == 14)) {
              ret = (ret + 249);
            }
            else {
            }
            if ((lpid >= 18) && (lpid <= 19)) {
              ret = (ret + 246);
            }
            else {
            }
            ret = (ret - 261);
          }
        }
      }
    }
    else {
      ( *(&std::cout)<<"Only support upto 4 cores ") << std::endl< char  , std::char_traits< char  >  > ;;
      (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1107,"false") : ((void )0);
    }
  }
  return g_tw_lp[ret];
}

int mapping_to_local_index(int lpid)
{
//    return lpid;
  int ret = lpid;
  if (tw_nnodes() == 2) {
    if (g_tw_mynode == 0) {
      if (lpid >= 20) {
        (__builtin_expect((!(lpid < 184)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1124,"lpid < 184") : ((void )0);
        ret = (ret - 10);
      }
      else {
      }
    }
    else {
      if ((lpid >= 10) && (lpid <= 19)) {
        ret = (ret + 164);
      }
      else {
      }
      ret = (ret - 174);
    }
  }
  else {
    if (tw_nnodes() == 4) {
      if (g_tw_mynode == 0) {
        if ((lpid == 3) || (lpid == 4)) {
          ret = (ret - 1);
        }
        else {
        }
        if (lpid == 6) {
          ret = (ret - 2);
        }
        else {
        }
        if ((lpid >= 20) && (lpid <= 101)) {
          ret = (ret - 15);
        }
        else {
        }
      }
      else {
        if (g_tw_mynode == 1) {
          if (lpid == 2) {
            ret = (ret + 85);
          }
          else {
          }
          if (lpid == 5) {
            ret = (ret + 83);
          }
          else {
          }
          if ((lpid >= 7) && (lpid <= 9)) {
            ret = (ret + 82);
          }
          else {
          }
          if ((lpid >= 102) && (lpid <= 183)) {
            ret = (ret - 10);
          }
          else {
          }
          ret = (ret - 87);
        }
        else {
          if (g_tw_mynode == 2) {
            if (lpid == 10) {
              ret = (ret + 164);
            }
            else {
            }
            if (lpid == 12) {
              ret = (ret + 163);
            }
            else {
            }
            if ((lpid >= 15) && (lpid <= 17)) {
              ret = (ret + 161);
            }
            else {
            }
            if ((lpid >= 184) && (lpid <= 265)) {
              ret = (ret - 5);
            }
            else {
            }
            ret = (ret - 174);
          }
          else {
            if (lpid == 11) {
              ret = (ret + 250);
            }
            else {
            }
            if ((lpid == 13) || (lpid == 14)) {
              ret = (ret + 249);
            }
            else {
            }
            if ((lpid >= 18) && (lpid <= 19)) {
              ret = (ret + 246);
            }
            else {
            }
            ret = (ret - 261);
          }
        }
      }
    }
    else {
      ( *(&std::cout)<<"Only support upto 4 cores ") << std::endl< char  , std::char_traits< char  >  > ;;
      (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1188,"false") : ((void )0);
    }
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
        tw_error("/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1215,"Attempting to mapping a KPid (%llu) for Global LPid %llu that is beyond g_tw_nkp (%llu)\n",kpid,lpid,g_tw_nkp);
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
//int loop_size = LOOP_SCALE * atoi(argv[argc-1].c_str());
//std::string ssize = argv[argc-1];
//int size = atoi(ssize.c_str());
//loop_size = LOOP_SCALE * size;
//std::cout<<"loop size : "<<loop_size<<std::endl;
//argv[argc-1] = "";
//argc = argc-1;
  int i;
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
  printf("loop size %d \n",loop_size);
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
  if (0) {
//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
    (std::cout << memusage<<",") << std::endl< char  , std::char_traits< char  >  > ;
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
                                        if ((airport >= 338) && (airport <= 347)) {
                                          return 19;;
                                        }
                                        else {
                                          class std::basic_ostream< char  , std::char_traits< char  >  > *__temp22__;
                                          __temp22__ = &( *(&std::cout)<<"airport region failed");
                                          ( *__temp22__) << std::endl< char  , std::char_traits< char  >  > ;;
                                          bool __temp23__;
                                          __temp23__ = !false;
                                          if ((__builtin_expect(__temp23__,0))) {
                                            __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1533,"false");;
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
