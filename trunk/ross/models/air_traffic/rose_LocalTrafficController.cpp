#include "LocalTrafficController.hpp"
#include "ross.h" 
#include "backstroke/stl_fwd_rvs_funcs.h" 
#include "rctypes.h" 

LocalTrafficController::LocalTrafficController(int in_max_capacity,int in_num_airport) : Controller(in_max_capacity,in_num_airport)
{
}

LocalTrafficController::~LocalTrafficController()
{
}

void LocalTrafficController::handle_incoming(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
}

void LocalTrafficController::handle_incoming_forward(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
}

void LocalTrafficController::handle_incoming_reverse(struct tw_lp *lp)
{
  --m_aircraft_processed;
  --m_current_capacity;
}

void LocalTrafficController::handle_outgoing(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_forward(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_reverse(struct tw_lp *lp)
{
  ++m_current_capacity;
}

void LocalTrafficController::handle_aircraft(struct tw_lp *lp)
{
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > *temp_q = ::new class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > ;
  class Aircraft old_top((this) -> m_q. top ());
  int old_size = ((this) -> m_q. size ());
  while(!(this) -> m_q. empty ()){
    class Aircraft temp((this) -> m_q. top ());
    (this) -> m_q. pop ();
    temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
    temp_q ->  push (temp);
  }
  while(!temp_q ->  empty ()){
    class Aircraft temp(temp_q ->  top ());
    temp_q ->  pop ();
    (this) -> m_q. push (temp);
  }
  class Aircraft new_top((this) -> m_q. top ());
  int new_size = ((this) -> m_q. size ());
  delete temp_q;
  bool __temp49__;
  __temp49__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
  if ((__builtin_expect(__temp49__,0))) {
    __assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/LocalTrafficController.cpp",56,"old_top.m_id == new_top.m_id");
  }
  else {
    (void )0;
  }
  bool __temp50__;
  __temp50__ = !(old_size == new_size);
  if ((__builtin_expect(__temp50__,0))) {
    __assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/LocalTrafficController.cpp",57,"old_size == new_size");
  }
  else {
    (void )0;
  }
}

void LocalTrafficController::handle_aircraft_forward(struct tw_lp *lp)
{
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > *temp_q = ::new class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > ;
  class Aircraft old_top((this) -> m_q. top ());
  int old_size = ((this) -> m_q. size ());
  __store__(m_q,lp);
  while(!(this) -> m_q. empty ()){
    class Aircraft temp((this) -> m_q. top ());
    (this) -> m_q. pop ();
    temp.Aircraft::m_cdelay++;
//cout<<temp.m_cdelay<<endl;
    temp_q ->  push (temp);
  }
  while(!temp_q ->  empty ()){
    class Aircraft temp(temp_q ->  top ());
    temp_q ->  pop ();
    (this) -> m_q. push (temp);
  }
  class Aircraft new_top((this) -> m_q. top ());
  int new_size = ((this) -> m_q. size ());
  delete temp_q;
  bool __temp49__;
  __temp49__ = !(old_top.Aircraft::m_id == new_top.Aircraft::m_id);
  if ((__builtin_expect(__temp49__,0))) {
    __assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/LocalTrafficController.cpp",56,"old_top.m_id == new_top.m_id");
  }
  else {
    (void )0;
  }
  bool __temp50__;
  __temp50__ = !(old_size == new_size);
  if ((__builtin_expect(__temp50__,0))) {
    __assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/LocalTrafficController.cpp",57,"old_size == new_size");
  }
  else {
    (void )0;
  }
}

void LocalTrafficController::handle_aircraft_reverse(struct tw_lp *lp)
{
  __restore__(m_q,lp);
}

Aircraft LocalTrafficController::get_aircraft(struct tw_lp *lp)
{
  class Aircraft ret((this) -> m_q. top ());
  return (ret);
}

Aircraft LocalTrafficController::get_aircraft_forward(struct tw_lp *lp)
{
  class Aircraft ret((this) -> m_q. top ());
  return (ret);
}

Aircraft LocalTrafficController::get_aircraft_reverse(struct tw_lp *lp)
{
}

void LocalTrafficController::remove_aircraft(struct tw_lp *lp)
{
  (this) -> m_q. pop ();
}

void LocalTrafficController::remove_aircraft_forward(struct tw_lp *lp)
{
  bs_priority_queue_pop_forward((this) -> m_q,lp);
}

void LocalTrafficController::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_priority_queue_pop_reverse((this) -> m_q,lp);
}

void LocalTrafficController::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push (aircraft);
}

void LocalTrafficController::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_priority_queue_push_forward((this) -> m_q,aircraft,lp);
}

void LocalTrafficController::add_aircraft_reverse(struct tw_lp *lp)
{
  bs_priority_queue_push_reverse((this) -> m_q,lp);
}
