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
        (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",72,"false") : ((void )0);
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
  (__builtin_expect((!( *(s -> airport_state::incoming_queue)). empty ()),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",90,"s->incoming_queue->empty()") : ((void )0);
  s -> airport_state::aircraft_counter = (::new class std::vector< int  , std::allocator< int  >  > (350));
  s -> airport_state::max_queue_size_airport = 0;
  s -> airport_state::max_queue_size_region = 0;
  s -> airport_state::delay_airport = 0;
  s -> airport_state::cdelay_airport = 0;
  s -> airport_state::delay_region = 0;
  s -> airport_state::cdelay_region = 0;
  if ((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER) {
    if (((((((lp -> tw_lp::gid) == 4) || ((lp -> tw_lp::gid) == 5)) || ((lp -> tw_lp::gid) == 8)) || ((lp -> tw_lp::gid) == 11)) || ((lp -> tw_lp::gid) == 13)) || ((lp -> tw_lp::gid) == 16)) {
      s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;
    }
    else {
      if ((((((((((lp -> tw_lp::gid) == 0) || ((lp -> tw_lp::gid) == 1)) || ((lp -> tw_lp::gid) == 3)) || ((lp -> tw_lp::gid) == 7)) || ((lp -> tw_lp::gid) == 10)) || ((lp -> tw_lp::gid) == 12)) || ((lp -> tw_lp::gid) == 14)) || ((lp -> tw_lp::gid) == 18)) || ((lp -> tw_lp::gid) == 19)) {
        s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;
      }
      else {
        s -> airport_state::max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
      }
    }
    s -> airport_state::airplane_in_region = 0;
    s -> airport_state::transit_req_accepted = 0;
    s -> airport_state::transit_req_rejected = 0;
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
    s -> airport_state::landing = 0;
    s -> airport_state::landing_req_accepted = 0;
    s -> airport_state::landing_req_rejected = 0;
    s -> airport_state::dep_processed = 0;
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
      bool __temp2__;
      __temp2__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp2__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",250,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
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
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::delay_airport += (tw_now(lp) - aircraft.Aircraft::m_clock);
        s -> airport_state::cdelay_airport += aircraft.Aircraft::m_cdelay;
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
        bool __temp3__;
        __temp3__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp3__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",317,"old_top.m_id == new_top.m_id");
        }
        else {
          (void )0;
        }
        bool __temp4__;
        __temp4__ = !(old_size == new_size);
        if ((__builtin_expect(__temp4__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",318,"old_size == new_size");
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
    if (((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway))?(( *(s -> airport_state::incoming_queue)). size () > 0) : true) {
      s -> airport_state::runway_in_use++;
      s -> airport_state::dep_processed++;
      class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
      ( *(s -> airport_state::incoming_queue)). pop ();
      s -> airport_state::delay_airport += (tw_now(lp) - aircraft.Aircraft::m_clock);
      s -> airport_state::cdelay_airport += aircraft.Aircraft::m_cdelay;
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
    goto LABEL11;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
//(*(s->aircraft_counters))[msg->aircraft.m_id]++;
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
        bool __temp5__;
        __temp5__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp5__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",392,"next_region == msg->aircraft.m_dest_region");
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
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL4:
{
      bool __temp6__;
      __temp6__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp6__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",435,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
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
      if ((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity)) {
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::incoming_queue)). pop ();
          s -> airport_state::airplane_in_region++;
          s -> airport_state::transit_req_accepted++;
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
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
        bool __temp7__;
        __temp7__ = !(( *(s -> airport_state::incoming_queue)). size () > 0);
        if ((__builtin_expect(__temp7__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",473,"s->incoming_queue->size() > 0");
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
        bool __temp8__;
        __temp8__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp8__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",508,"old_top.m_id == new_top.m_id");
        }
        else {
          (void )0;
        }
        bool __temp9__;
        __temp9__ = !(old_size == new_size);
        if ((__builtin_expect(__temp9__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",509,"old_size == new_size");
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
    if (((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity))?(( *(s -> airport_state::incoming_queue)). size () > 0) : true) {
      class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
      if (aircraft.Aircraft::m_remaining_dist <= 0) {
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::airplane_in_region++;
//s->transit_req_accepted++;
        s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
        s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
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
//s->transit_req_rejected++;
    }
//update remaining distance, process time, delays
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
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
      bool __temp10__;
      __temp10__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp10__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",582,"old_top.m_id == new_top.m_id");
      }
      else {
        (void )0;
      }
      bool __temp11__;
      __temp11__ = !(old_size == new_size);
      if ((__builtin_expect(__temp11__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",583,"old_size == new_size");
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
      bool __temp12__;
      __temp12__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp12__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",596,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        (void )0;
      }
      s -> airport_state::airplane_in_region--;
      int src_region = (lp -> tw_lp::gid);
      int next_region = 0;
      class std::deque< int  , std::allocator< int  >  > path = graph ->  get_shortest_path (src_region,msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
      if (path. size () != 1) {
        path. pop_front ();
        next_region = path. front ();
      }
      else {
        next_region = path. front ();
        bool __temp13__;
        __temp13__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp13__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",612,"next_region == msg->aircraft.m_dest_region");
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
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      goto LABEL11;
    }
    LABEL7:
{
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
        s -> airport_state::max_queue_size_airport = (s -> airport_state::max_queue_size_airport);
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_req_accepted++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        int to = (lp -> tw_lp::gid);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = LANDING;
        m -> air_traffic_message::aircraft = (aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      else {
        s -> airport_state::landing_req_rejected++;
//update delays
      }
      goto LABEL11;
    }
    LABEL8:
{
      int to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAXI_IN;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL9:
{
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
      int __temp14__;
      __temp14__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp14__));
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
      bool __temp2__;
      __temp2__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
      if ((__builtin_expect(__temp2__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",250,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");
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
        s -> airport_state::dep_processed++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        __store__(s -> airport_state::delay_airport,lp);
        s -> airport_state::delay_airport += (tw_now(lp) - aircraft.Aircraft::m_clock);
        __store__< int  > (s -> airport_state::cdelay_airport,lp);
        s -> airport_state::cdelay_airport += aircraft.Aircraft::m_cdelay;
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
        bool __temp3__;
        __temp3__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp3__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",317,"old_top.m_id == new_top.m_id");
        }
        else {
          __num0 += 2;
          (void )0;
        }
        bool __temp4__;
        __temp4__ = !(old_size == new_size);
        if ((__builtin_expect(__temp4__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",318,"old_size == new_size");
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
    if (((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway))?(( *(s -> airport_state::incoming_queue)). size () > 0) : ((__num0 += 2 , true))) {
      s -> airport_state::runway_in_use++;
      s -> airport_state::dep_processed++;
      class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
      __store__( *s -> airport_state::incoming_queue,lp);
      ( *(s -> airport_state::incoming_queue)). pop ();
      __store__(s -> airport_state::delay_airport,lp);
      s -> airport_state::delay_airport += (tw_now(lp) - aircraft.Aircraft::m_clock);
      __store__< int  > (s -> airport_state::cdelay_airport,lp);
      s -> airport_state::cdelay_airport += aircraft.Aircraft::m_cdelay;
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
      __num0 += 1;
    }
    goto LABEL11;
    LABEL2:
{
//cout<<s->aircraft_counters->size()<<","<<msg->aircraft.m_id<<endl;
//(*(s->aircraft_counters))[msg->aircraft.m_id]++;
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
        bool __temp5__;
        __temp5__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp5__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",392,"next_region == msg->aircraft.m_dest_region");
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
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
      e = tw_event_new(to2,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = DEP;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL4:
{
      bool __temp6__;
      __temp6__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp6__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",435,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        __num0 += 64;
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
        __num0 += 32;
      }
      if ((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity)) {
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        if (aircraft.Aircraft::m_remaining_dist <= 0) {
          ( *(s -> airport_state::incoming_queue)). pop ();
          s -> airport_state::airplane_in_region++;
          s -> airport_state::transit_req_accepted++;
          __store__(s -> airport_state::delay_region,lp);
          s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
          __store__< int  > (s -> airport_state::cdelay_region,lp);
          s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
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
          __num0 += 8;
        }
      }
      else {
        __num0 += 16;
        bool __temp7__;
        __temp7__ = !(( *(s -> airport_state::incoming_queue)). size () > 0);
        if ((__builtin_expect(__temp7__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",473,"s->incoming_queue->size() > 0");
        }
        else {
          __num0 += 8;
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
        bool __temp8__;
        __temp8__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
        if ((__builtin_expect(__temp8__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",508,"old_top.m_id == new_top.m_id");
        }
        else {
          __num0 += 2;
          (void )0;
        }
        bool __temp9__;
        __temp9__ = !(old_size == new_size);
        if ((__builtin_expect(__temp9__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",509,"old_size == new_size");
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
    LABEL5:
    if (((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity))?(( *(s -> airport_state::incoming_queue)). size () > 0) : ((__num0 += 32 , true))) {
      class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
      if (aircraft.Aircraft::m_remaining_dist <= 0) {
        __store__( *s -> airport_state::incoming_queue,lp);
        ( *(s -> airport_state::incoming_queue)). pop ();
        s -> airport_state::airplane_in_region++;
//s->transit_req_accepted++;
        __store__(s -> airport_state::delay_region,lp);
        s -> airport_state::delay_region += (tw_now(lp) - aircraft.Aircraft::m_clock);
        __store__< int  > (s -> airport_state::cdelay_region,lp);
        s -> airport_state::cdelay_region += aircraft.Aircraft::m_cdelay;
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
        __num0 += 8;
      }
    }
    else {
      __num0 += 16;
//s->transit_req_rejected++;
    }
//update remaining distance, process time, delays
    if (( *(s -> airport_state::incoming_queue)). size () > 0) {
      class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > *temp_q = ::new class std::priority_queue< Aircraft  , std::vector< Aircraft  , std::allocator< Aircraft  >  >  , std::less< Aircraft  >  > ;
      class Aircraft old_top(( *(s -> airport_state::incoming_queue)). top ());
      int old_size = (( *(s -> airport_state::incoming_queue)). size ());
      __store__( *s -> airport_state::incoming_queue,lp);
      while(!( *(s -> airport_state::incoming_queue)). empty ()){
        class Aircraft temp(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
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
      bool __temp10__;
      __temp10__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
      if ((__builtin_expect(__temp10__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",582,"old_top.m_id == new_top.m_id");
      }
      else {
        __num0 += 2;
        (void )0;
      }
      bool __temp11__;
      __temp11__ = !(old_size == new_size);
      if ((__builtin_expect(__temp11__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",583,"old_size == new_size");
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
    LABEL6:
{
      bool __temp12__;
      __temp12__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
      if ((__builtin_expect(__temp12__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",596,"lp->gid < NUMBER_OF_REGION_CONTROLLER");
      }
      else {
        __num0 += 16;
        (void )0;
      }
      s -> airport_state::airplane_in_region--;
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
        bool __temp13__;
        __temp13__ = !(next_region == msg -> air_traffic_message::aircraft.Aircraft::m_dest_region);
        if ((__builtin_expect(__temp13__,0))) {
          __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",612,"next_region == msg->aircraft.m_dest_region");
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
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      goto LABEL11;
    }
    LABEL7:
{
      class Aircraft aircraft(msg -> air_traffic_message::aircraft);
      aircraft.Aircraft::m_clock = tw_now(lp);
      __store__( *s -> airport_state::incoming_queue,lp);
      ( *(s -> airport_state::incoming_queue)). push (aircraft);
      if (( *(s -> airport_state::incoming_queue)). size () > (s -> airport_state::max_queue_size_airport)) {
        __store__< int  > (s -> airport_state::max_queue_size_airport,lp);
        s -> airport_state::max_queue_size_airport = (( *(s -> airport_state::incoming_queue)). size ());
      }
      else {
        __num0 += 2;
        s -> airport_state::max_queue_size_airport = (s -> airport_state::max_queue_size_airport);
      }
      if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
        s -> airport_state::runway_in_use++;
        s -> airport_state::landing_req_accepted++;
        class Aircraft aircraft(( *(s -> airport_state::incoming_queue)). top ());
        ( *(s -> airport_state::incoming_queue)). pop ();
        int to = (lp -> tw_lp::gid);
        __store__< int  > (s -> airport_state::rn,lp);
        ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
        e = tw_event_new(to,ts,lp);
        m = ((air_traffic_message *)(tw_event_data(e)));
        m -> air_traffic_message::type = LANDING;
        m -> air_traffic_message::aircraft = (aircraft);
        m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
        tw_event_send(e);
      }
      else {
        __num0 += 1;
        s -> airport_state::landing_req_rejected++;
//update delays
      }
      goto LABEL11;
    }
    LABEL8:
{
      int to = (lp -> tw_lp::gid);
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
      e = tw_event_new(to,ts,lp);
      m = ((air_traffic_message *)(tw_event_data(e)));
      m -> air_traffic_message::type = TAXI_IN;
      m -> air_traffic_message::aircraft = (msg -> air_traffic_message::aircraft);
      m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
      tw_event_send(e);
      goto LABEL11;
    }
    LABEL9:
{
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
      int __temp14__;
      __temp14__ = (NUMBER_OF_LP - 1);
      int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp14__));
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
  if ((__num0 & 130816) == 130560) {
    __restore__(s -> airport_state::rn,lp);
  }
  else {
    if ((__num0 & 131008) == 130944) {
      __restore__(s -> airport_state::rn,lp);
      ++s -> airport_state::runway_in_use;
    }
    else {
      if ((__num0 & 130944) == 130816) {
        __restore__(s -> airport_state::rn,lp);
      }
      else {
        if ((__num0 & 98304) == 65536) {
          if ((__num0 & 98305) == 65537) {
          }
          else {
            __restore__(s -> airport_state::rn,lp);
            __restore__(s -> airport_state::cdelay_airport,lp);
            __restore__(s -> airport_state::delay_airport,lp);
            __restore__( *s -> airport_state::incoming_queue,lp);
            --s -> airport_state::dep_processed;
            --s -> airport_state::runway_in_use;
          }
        }
        else {
          if ((__num0 & 130560) == 130048) {
            if ((__num0 & 130561) == 130049) {
              --s -> airport_state::landing_req_rejected;
            }
            else {
              __restore__(s -> airport_state::rn,lp);
              --s -> airport_state::landing_req_accepted;
              --s -> airport_state::runway_in_use;
            }
            if ((__num0 & 130562) == 130050) {
            }
            else {
              __restore__(s -> airport_state::max_queue_size_airport,lp);
            }
            __restore__( *s -> airport_state::incoming_queue,lp);
          }
          else {
            if ((__num0 & 122880) == 114688) {
              __restore__(s -> airport_state::rn,lp);
              ++s -> airport_state::runway_in_use;
            }
            else {
              if ((__num0 & 130048) == 129024) {
                if ((__num0 & 130050) == 129024) {
                  __restore__(s -> airport_state::rn,lp);
                }
                else {
                  __restore__(s -> airport_state::rn,lp);
                }
                ++s -> airport_state::airplane_in_region;
              }
              else {
                if ((__num0 & 129024) == 126976) {
                  if ((__num0 & 129028) == 126976) {
                    __restore__( *s -> airport_state::incoming_queue,lp);
                  }
                  else {
                  }
                  if ((__num0 & 129040) == 126976) {
                    if ((__num0 & 129048) == 126984) {
                    }
                    else {
                      __restore__(s -> airport_state::rn,lp);
                      __restore__(s -> airport_state::cdelay_region,lp);
                      __restore__(s -> airport_state::delay_region,lp);
                      --s -> airport_state::airplane_in_region;
                      __restore__( *s -> airport_state::incoming_queue,lp);
                    }
                  }
                  else {
                  }
                }
                else {
                  if ((__num0 & 126976) == 122880) {
                    if ((__num0 & 126992) == 122880) {
                      if ((__num0 & 127000) == 122888) {
                      }
                      else {
                        __restore__(s -> airport_state::rn,lp);
                        __restore__(s -> airport_state::cdelay_region,lp);
                        __restore__(s -> airport_state::delay_region,lp);
                        --s -> airport_state::transit_req_accepted;
                        --s -> airport_state::airplane_in_region;
                      }
                    }
                    else {
                      --s -> airport_state::transit_req_rejected;
                    }
                    if ((__num0 & 127008) == 122912) {
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
                        __restore__(s -> airport_state::cdelay_airport,lp);
                        __restore__(s -> airport_state::delay_airport,lp);
                        --s -> airport_state::dep_processed;
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

void final(airport_state *s,tw_lp *lp)
{
//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
  total_transit_accepted += (s -> airport_state::transit_req_accepted);
  total_transit_rejected += (s -> airport_state::transit_req_rejected);
  total_dep_processed += (s -> airport_state::dep_processed);
  total_dep_req_rejected += (s -> airport_state::dep_req_rejected);
  total_landing_req_accepted += (s -> airport_state::landing_req_accepted);
  total_landing_req_rejected += (s -> airport_state::landing_req_rejected);
  max_queue_size_airport = (((s -> airport_state::max_queue_size_airport) > max_queue_size_airport)?(s -> airport_state::max_queue_size_airport) : max_queue_size_airport);
  max_queue_size_region = (((s -> airport_state::max_queue_size_region) > max_queue_size_region)?(s -> airport_state::max_queue_size_region) : max_queue_size_region);
  total_cdelay_airport += (s -> airport_state::cdelay_airport);
  total_delay_airport += (s -> airport_state::delay_airport);
  total_cdelay_region += (s -> airport_state::cdelay_region);
  total_delay_region += (s -> airport_state::delay_region);
//cout<<lp->gid<<","<<total_cdelay<<endl;
}
/*
   Parallel Running
 */
tw_lptype airport_lps[] = {{((init_f )init), ((event_f )event_handler), ((revent_f )event_handler), ((final_f )final), ((map_f )mapping_to_pe), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_STIME), ("mean"), ("mean flight time for planes"), ((&mean_flight_time))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, {(TWOPTTYPE_UINT), ("loopsize"), ("padding loop size"), ((&loop_size))}, {(TWOPTTYPE_UINT), ("prun"), ("parallel run"), ((&p_run))}, {((tw_opttype )0), (0L), (0L), (0L)}};

tw_lp *mapping_to_lp(tw_lpid lpid)
{
  int ret = lpid;
  if (tw_nnodes() == 2) {
    if (g_tw_mynode == 0) {
      if (lpid >= 20) {
        (__builtin_expect((!(lpid < 184)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1574,"lpid < 184") : ((void )0);
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
      (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1638,"false") : ((void )0);
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
        (__builtin_expect((!(lpid < 184)),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1655,"lpid < 184") : ((void )0);
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
      (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1719,"false") : ((void )0);
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
        tw_error("/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1746,"Attempting to mapping a KPid (%llu) for Global LPid %llu that is beyond g_tw_nkp (%llu)\n",kpid,lpid,g_tw_nkp);
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
    (( *(&std::cout)<<"\tTotal Transit Accepted : ") << total_transit_accepted) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Transit Rejected : ") << total_transit_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Departure Processed : ") << total_dep_processed) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Departure Req Rejected : ") << total_dep_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Arrival Req Accepted : ") << total_landing_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tTotal Arrival Req Rejected : ") << total_landing_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport cdelay : ") << (((double )total_cdelay_airport) / total_dep_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Airport delay : ") << (total_delay_airport / total_dep_processed)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Region cdelay : ") << (((double )total_cdelay_region) / total_transit_accepted)) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg Region delay : ") << (total_delay_region / total_transit_accepted)) << std::endl< char  , std::char_traits< char  >  > ;
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
                                          class std::basic_ostream< char  , std::char_traits< char  >  > *__temp15__;
                                          __temp15__ = &( *(&std::cout)<<"airport region failed");
                                          ( *__temp15__) << std::endl< char  , std::char_traits< char  >  > ;;
                                          bool __temp16__;
                                          __temp16__ = !false;
                                          if ((__builtin_expect(__temp16__,0))) {
                                            __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",2051,"false");;
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
