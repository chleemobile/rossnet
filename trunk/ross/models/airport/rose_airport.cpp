#include "airport.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>
/*
 	
   Chayong Lee
   06012012
 */
#include "rctypes.h" 

tw_peid mapping(tw_lpid gid)
{
  return ((tw_peid )gid) / g_tw_nlp;
}

void init(airport_state *s,tw_lp *lp)
{
  struct BSStack *stack = ::new BSStack ;
  lp -> tw_lp::stack_pointer = stack;
  int i;
  tw_lpid evnt_to;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  s -> airport_state::rn = (lp -> tw_lp::gid);
  s -> airport_state::current_capacity = 0;
  s -> airport_state::max_capacity = 1;
  s -> airport_state::dep_processed = 0;
  s -> airport_state::dep_queued = 0;
  s -> airport_state::wdelay = 0;
  s -> airport_state::sdelay = 0;
  for (i = 0; i < planes_per_airport; i++) {
    evnt_to = (lp -> tw_lp::gid);
    ts = bs_rand_exponential(s -> airport_state::rn,30.0);
    int aircraft_dest = (bs_rand_integer(s -> airport_state::rn,0,(1024 - 1)));
    e = tw_event_new(evnt_to,ts,lp);
    class Aircraft int_aircraft;
    m = ((airport_message *)(tw_event_data(e)));
    m -> airport_message::type = DEPARTURE;
    m -> airport_message::aircraft = (int_aircraft);
    m -> airport_message::aircraft.Aircraft::m_dest = aircraft_dest;
    tw_event_send(e);
  }
}

void event_handler(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  tw_lpid evnt_to;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
{
    int __item_selector__ = (int )(msg -> airport_message::type);
    if (__item_selector__ == ARRIVAL) {
      goto LABEL0;;
    }
    else {
      if (__item_selector__ == DEPARTURE) {
        goto LABEL1;;
      }
      else {
        if (__item_selector__ == LAND) {
          goto LABEL2;;
        }
        else {;
        };
      };
    }
    LABEL0:
// Schedule a landing in the future	
//printf("ARRIVAL aircraft %d arrived \n", msg->aircraft.get_id());
    evnt_to = (lp -> tw_lp::gid);
    ts = bs_rand_exponential(s -> airport_state::rn,10.0);
    e = tw_event_new((lp -> tw_lp::gid),ts,lp);
    m = ((airport_message *)(tw_event_data(e)));
    m -> airport_message::type = LAND;
    m -> airport_message::aircraft = (msg -> airport_message::aircraft);
    tw_event_send(e);
    goto LABEL3;;;
    LABEL1:
//cout<<"handling "<<msg->aircraft.get_id()<<endl;
    msg -> airport_message::aircraft.Aircraft::m_wclock = tw_now(lp);
    s -> airport_state::queue. push_back (msg -> airport_message::aircraft);
    if ((s -> airport_state::current_capacity) < (s -> airport_state::max_capacity)) {
      s -> airport_state::dep_processed++;
      s -> airport_state::current_capacity++;
      bool __temp0__;
      __temp0__ = !(s -> airport_state::queue. size () > 0);
      if ((__builtin_expect(__temp0__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/airport.cpp",96,"s->queue.size() > 0");;
      }
      else {
        (void )0;;
      };
//printf("DEP handling %d aircraft \n", t_aircraft.get_id());
      class Aircraft t_aircraft(s -> airport_state::queue. front ());
      std::vector< Aircraft ,std::allocator< Aircraft  > > ::iterator __temp1__;
      __temp1__ = s -> airport_state::queue. begin ();
      s -> airport_state::queue. erase (__temp1__);;
      s -> airport_state::wdelay += t_aircraft.Aircraft::m_wdelay;
      s -> airport_state::sdelay += t_aircraft.Aircraft::m_sdelay;
      t_aircraft.Aircraft::m_wdelay = 0;
      t_aircraft.Aircraft::m_sdelay = 0;
      t_aircraft.Aircraft::m_wclock = 0;
      evnt_to = t_aircraft.Aircraft::m_dest;
      ts = bs_rand_exponential(s -> airport_state::rn,20.0);
      e = tw_event_new(evnt_to,ts,lp);
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = ARRIVAL;
      m -> airport_message::aircraft = (t_aircraft);
      tw_event_send(e);;
    }
    else {
      s -> airport_state::dep_queued++;
//cout<<"queuing "<<msg->aircraft.get_id()<<"("<<s->queue.size()<<")"<<endl;
//vector<Aircraft>::iterator itr = s->queue.begin(); 
      int temp_size = (s -> airport_state::queue. size ());
      int temp_i = 0;;
      while(temp_i < temp_size){
//s->queue[temp_i].calculate_wdelay(tw_now(lp));
        double t_now = tw_now(lp);
        double t_wdelay = s -> airport_state::queue[temp_i].Aircraft::m_wdelay;
        double t_wclock = s -> airport_state::queue[temp_i].Aircraft::m_wclock;
        t_wdelay = (t_wdelay + (t_now - t_wclock));
        s -> airport_state::queue[temp_i].Aircraft::m_wdelay = t_wdelay;
        s -> airport_state::queue[temp_i].Aircraft::m_wclock = t_wclock;
        s -> airport_state::queue[temp_i].Aircraft::m_sdelay++;
        temp_i++;;
//printf("aircraft %d has been waiting %f, %d\n",(*itr).get_id(), (*itr).get_wdelay(), (*itr).get_sdelay());
      };
//cout<<""<<endl;
    }
    goto LABEL3;;;
    LABEL2:
{
//printf("LAND aircraft %d arrived \n", msg->aircraft.get_id());
      s -> airport_state::current_capacity--;
      evnt_to = (lp -> tw_lp::gid);
      ts = bs_rand_exponential(s -> airport_state::rn,20.0);
      e = tw_event_new(evnt_to,ts,lp);
      int __temp2__;
      __temp2__ = (1024 - 1);
      int aircraft_dest = (bs_rand_integer(s -> airport_state::rn,0,__temp2__));
      class Aircraft int_aircraft;
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = DEPARTURE;
      m -> airport_message::aircraft = (int_aircraft);
      m -> airport_message::aircraft.Aircraft::m_dest = aircraft_dest;
      tw_event_send(e);
      goto LABEL3;;
    };
    LABEL3:;;;
  }
  s -> airport_state::rn;
  s -> airport_state::max_capacity;
  s -> airport_state::current_capacity;
  s -> airport_state::dep_processed;
  s -> airport_state::dep_queued;
  s -> airport_state::queue;
  s -> airport_state::wdelay;
  s -> airport_state::sdelay;
  bf -> tw_bf::c0;
  bf -> tw_bf::c1;
  bf -> tw_bf::c2;
  bf -> tw_bf::c3;
  bf -> tw_bf::c4;
  bf -> tw_bf::c5;
  bf -> tw_bf::c6;
  bf -> tw_bf::c7;
  bf -> tw_bf::c8;
  bf -> tw_bf::c9;
  bf -> tw_bf::c10;
  bf -> tw_bf::c11;
  bf -> tw_bf::c12;
  bf -> tw_bf::c13;
  bf -> tw_bf::c14;
  bf -> tw_bf::c15;
  bf -> tw_bf::c16;
  bf -> tw_bf::c17;
  bf -> tw_bf::c18;
  bf -> tw_bf::c19;
  bf -> tw_bf::c20;
  bf -> tw_bf::c21;
  bf -> tw_bf::c22;
  bf -> tw_bf::c23;
  bf -> tw_bf::c24;
  bf -> tw_bf::c25;
  bf -> tw_bf::c26;
  bf -> tw_bf::c27;
  bf -> tw_bf::c28;
  bf -> tw_bf::c29;
  bf -> tw_bf::c30;
  bf -> tw_bf::c31;
  msg -> airport_message::type;
  msg -> airport_message::aircraft;
  msg -> airport_message::msg_from;
  lp -> tw_lp::id;
  lp -> tw_lp::gid;
  lp -> tw_lp::pe;
  lp -> tw_lp::kp;
  lp -> tw_lp::cur_state;
  lp -> tw_lp::state_qh;
  lp -> tw_lp::type;
  lp -> tw_lp::rng;
  lp -> tw_lp::stack_pointer;
  lp -> tw_lp::current_seed;
  lp -> tw_lp::seed_head;
  lp -> tw_lp::seed_tail;;
}

void event_handler_forward(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int __num0 = 0;
  tw_lpid evnt_to;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
{
    int __item_selector__ = (int )(msg -> airport_message::type);
    if (__item_selector__ == ARRIVAL) {
      goto LABEL0;;
    }
    else {
      __num0 += 8;
      if (__item_selector__ == DEPARTURE) {
        goto LABEL1;;
      }
      else {
        __num0 += 4;
        if (__item_selector__ == LAND) {
          goto LABEL2;;
        }
        else {
          __num0 += 1;;
        };
      };
    }
    LABEL0:
// Schedule a landing in the future	
//printf("ARRIVAL aircraft %d arrived \n", msg->aircraft.get_id());
    evnt_to = (lp -> tw_lp::gid);
    __store__< int  > (s -> airport_state::rn,lp);
    ts = bs_rand_exponential(s -> airport_state::rn,10.0);
    e = tw_event_new((lp -> tw_lp::gid),ts,lp);
    m = ((airport_message *)(tw_event_data(e)));
    m -> airport_message::type = LAND;
    m -> airport_message::aircraft = (msg -> airport_message::aircraft);
    tw_event_send(e);
    goto LABEL3;;;
    LABEL1:
//cout<<"handling "<<msg->aircraft.get_id()<<endl;
    msg -> airport_message::aircraft.Aircraft::m_wclock = tw_now(lp);
    __store__(s -> airport_state::queue,lp);
    s -> airport_state::queue. push_back (msg -> airport_message::aircraft);
    if ((s -> airport_state::current_capacity) < (s -> airport_state::max_capacity)) {
      s -> airport_state::dep_processed++;
      s -> airport_state::current_capacity++;
      bool __temp0__;
      __temp0__ = !(s -> airport_state::queue. size () > 0);
      if ((__builtin_expect(__temp0__,0))) {
        __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/airport.cpp",96,"s->queue.size() > 0");;
      }
      else {
        __num0 += 1;
        (void )0;;
      };
//printf("DEP handling %d aircraft \n", t_aircraft.get_id());
      class Aircraft t_aircraft(s -> airport_state::queue. front ());
      std::vector< Aircraft ,std::allocator< Aircraft  > > ::iterator __temp1__;
      __temp1__ = s -> airport_state::queue. begin ();
      s -> airport_state::queue. erase (__temp1__);;
      __store__(s -> airport_state::wdelay,lp);
      s -> airport_state::wdelay += t_aircraft.Aircraft::m_wdelay;
      __store__< int  > (s -> airport_state::sdelay,lp);
      s -> airport_state::sdelay += t_aircraft.Aircraft::m_sdelay;
      t_aircraft.Aircraft::m_wdelay = 0;
      t_aircraft.Aircraft::m_sdelay = 0;
      t_aircraft.Aircraft::m_wclock = 0;
      evnt_to = t_aircraft.Aircraft::m_dest;
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,20.0);
      e = tw_event_new(evnt_to,ts,lp);
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = ARRIVAL;
      m -> airport_message::aircraft = (t_aircraft);
      tw_event_send(e);;
    }
    else {
      __num0 += 2;
      s -> airport_state::dep_queued++;
//cout<<"queuing "<<msg->aircraft.get_id()<<"("<<s->queue.size()<<")"<<endl;
//vector<Aircraft>::iterator itr = s->queue.begin(); 
      int temp_size = (s -> airport_state::queue. size ());
      int temp_i = 0;;
      while(temp_i < temp_size){
//s->queue[temp_i].calculate_wdelay(tw_now(lp));
        double t_now = tw_now(lp);
        double t_wdelay = s -> airport_state::queue[temp_i].Aircraft::m_wdelay;
        double t_wclock = s -> airport_state::queue[temp_i].Aircraft::m_wclock;
        t_wdelay = (t_wdelay + (t_now - t_wclock));
        s -> airport_state::queue[temp_i].Aircraft::m_wdelay = t_wdelay;
        s -> airport_state::queue[temp_i].Aircraft::m_wclock = t_wclock;
        s -> airport_state::queue[temp_i].Aircraft::m_sdelay++;
        temp_i++;;
//printf("aircraft %d has been waiting %f, %d\n",(*itr).get_id(), (*itr).get_wdelay(), (*itr).get_sdelay());
      };
//cout<<""<<endl;
    }
    goto LABEL3;;;
    LABEL2:
{
//printf("LAND aircraft %d arrived \n", msg->aircraft.get_id());
      s -> airport_state::current_capacity--;
      evnt_to = (lp -> tw_lp::gid);
      __store__< int  > (s -> airport_state::rn,lp);
      ts = bs_rand_exponential(s -> airport_state::rn,20.0);
      e = tw_event_new(evnt_to,ts,lp);
      int __temp2__;
      __temp2__ = (1024 - 1);
      int aircraft_dest = (bs_rand_integer(s -> airport_state::rn,0,__temp2__));
      class Aircraft int_aircraft;
      m = ((airport_message *)(tw_event_data(e)));
      m -> airport_message::type = DEPARTURE;
      m -> airport_message::aircraft = (int_aircraft);
      m -> airport_message::aircraft.Aircraft::m_dest = aircraft_dest;
      tw_event_send(e);
      goto LABEL3;;
    };
    LABEL3:;;;
  }
  s -> airport_state::rn;
  s -> airport_state::max_capacity;
  s -> airport_state::current_capacity;
  s -> airport_state::dep_processed;
  s -> airport_state::dep_queued;
  s -> airport_state::queue;
  s -> airport_state::wdelay;
  s -> airport_state::sdelay;
  bf -> tw_bf::c0;
  bf -> tw_bf::c1;
  bf -> tw_bf::c2;
  bf -> tw_bf::c3;
  bf -> tw_bf::c4;
  bf -> tw_bf::c5;
  bf -> tw_bf::c6;
  bf -> tw_bf::c7;
  bf -> tw_bf::c8;
  bf -> tw_bf::c9;
  bf -> tw_bf::c10;
  bf -> tw_bf::c11;
  bf -> tw_bf::c12;
  bf -> tw_bf::c13;
  bf -> tw_bf::c14;
  bf -> tw_bf::c15;
  bf -> tw_bf::c16;
  bf -> tw_bf::c17;
  bf -> tw_bf::c18;
  bf -> tw_bf::c19;
  bf -> tw_bf::c20;
  bf -> tw_bf::c21;
  bf -> tw_bf::c22;
  bf -> tw_bf::c23;
  bf -> tw_bf::c24;
  bf -> tw_bf::c25;
  bf -> tw_bf::c26;
  bf -> tw_bf::c27;
  bf -> tw_bf::c28;
  bf -> tw_bf::c29;
  bf -> tw_bf::c30;
  bf -> tw_bf::c31;
  msg -> airport_message::type;
  msg -> airport_message::aircraft;
  msg -> airport_message::msg_from;
  lp -> tw_lp::id;
  lp -> tw_lp::gid;
  lp -> tw_lp::pe;
  lp -> tw_lp::kp;
  lp -> tw_lp::cur_state;
  lp -> tw_lp::state_qh;
  lp -> tw_lp::type;
  lp -> tw_lp::rng;
  lp -> tw_lp::stack_pointer;
  lp -> tw_lp::current_seed;
  lp -> tw_lp::seed_head;
  lp -> tw_lp::seed_tail;;
  __store__< int  > (__num0,lp);
}

void event_handler_reverse(airport_state *s,tw_bf *bf,airport_message *msg,tw_lp *lp)
{
  int __num0;
  __restore__(__num0,lp);
  if ((__num0 & 12) == 8) {
    if ((__num0 & 14) == 10) {
      --s -> airport_state::dep_queued;
    }
    else {
      __restore__(s -> airport_state::rn,lp);
      __restore__(s -> airport_state::sdelay,lp);
      __restore__(s -> airport_state::wdelay,lp);
      --s -> airport_state::current_capacity;
      --s -> airport_state::dep_processed;
    }
    __restore__(s -> airport_state::queue,lp);
  }
  else {
    if ((__num0 & 8) == 0 || (__num0 & 13) == 13) {
      __restore__(s -> airport_state::rn,lp);
    }
    else {
      __restore__(s -> airport_state::rn,lp);
      ++s -> airport_state::current_capacity;
    }
  }
}

void final(airport_state *s,tw_lp *lp)
{
  ttl_wdelay += (s -> airport_state::wdelay);
  ttl_sdelay += (s -> airport_state::sdelay);
  ttl_dep_processed += (s -> airport_state::dep_processed);
  ttl_dep_queued += (s -> airport_state::dep_queued);
  avg_wdelay = (ttl_wdelay / ttl_dep_processed);
  avg_sdelay = (((double )ttl_sdelay) / ttl_dep_processed);
}
tw_lptype airport_lps[] = {{((init_f )init), ((event_f )event_handler_forward), ((revent_f )event_handler_reverse), ((final_f )final), ((map_f )mapping), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, {((tw_opttype )0), (0L), (0L), (0L)}};

int main(int argc,char **argv,char **env)
{
  int i;
  tw_opt_add(app_opt);
  tw_init(&argc,&argv);
  nlp_per_pe /= ((tw_nnodes()) * g_tw_npe);
  g_tw_events_per_pe = (((planes_per_airport * nlp_per_pe) / g_tw_npe) + opt_mem);
  tw_define_lps(nlp_per_pe,(sizeof(airport_message )),0);
  for (i = 0; i < g_tw_nlp; i++) {
    tw_lp_settype(i,(airport_lps + 0));
  }
  tw_run();
  if ((tw_ismaster())) {
    printf("\nAirport Model Statistics:\n");
    printf("\t%-50s %11lld\n","Number of airports",((nlp_per_pe * g_tw_npe) * (tw_nnodes())));
    printf("\t%-50s %11lld\n","Number of planes",(((planes_per_airport * nlp_per_pe) * g_tw_npe) * (tw_nnodes())));
    (( *(&std::cout)<<"\tdep processed : ") << ttl_dep_processed) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tdep delayed : ") << ttl_dep_queued) << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg wdelay : ") << avg_wdelay<<" in dep queue") << std::endl< char  , std::char_traits< char  >  > ;
    (( *(&std::cout)<<"\tAvg sdelay : ") << avg_sdelay<<" in dep queue") << std::endl< char  , std::char_traits< char  >  > ;
  }
  else {
  }
  tw_end();
  if (1) {
//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
    (std::cout << memusage<<",") << std::endl< char  , std::char_traits< char  >  > ;
  }
  else {
  }
  return 0;
}
