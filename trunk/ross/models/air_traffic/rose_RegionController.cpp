#include "RegionController.hpp"
#include <ross.h>
#include "rctypes.h" 

RegionController::RegionController(int in_max_capacity)
{
  (this) -> m_max_capacity = in_max_capacity;
  (this) -> m_current_capacity = 0;
  (this) -> dummy_test = 1.2345;
}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_forward(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_reverse(struct tw_lp *lp)
{
  --m_current_capacity;
}

void RegionController::handle_outgoing(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_forward(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_reverse(struct tw_lp *lp)
{
  ++m_current_capacity;
}

void RegionController::handle_aircraft(struct tw_lp *lp)
{
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > *temp_q = ::new class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > ;
  class Aircraft old_top((this) -> m_q. top ());
  int old_size = ((this) -> m_q. size ());
  while(!(this) -> m_q. empty ()){
    class Aircraft temp((this) -> m_q. top ());
    (this) -> m_q. pop ();
    temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
    temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
    temp.Aircraft::m_cdelay++;
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
//assert(old_top.m_id == new_top.m_id);
  bool __temp40__;
  __temp40__ = !(old_size == new_size);
  if ((__builtin_expect(__temp40__,0))) {
    __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/RegionController.cpp",62,"old_size == new_size");
  }
  else {
    (void )0;
  }
}

void RegionController::handle_aircraft_forward(struct tw_lp *lp)
{
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > *temp_q = ::new class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > ;
  class Aircraft old_top((this) -> m_q. top ());
  int old_size = ((this) -> m_q. size ());
  __store__(m_q,lp);
  while(!(this) -> m_q. empty ()){
    class Aircraft temp((this) -> m_q. top ());
    (this) -> m_q. pop ();
    temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
    temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
    temp.Aircraft::m_cdelay++;
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
//assert(old_top.m_id == new_top.m_id);
  bool __temp40__;
  __temp40__ = !(old_size == new_size);
  if ((__builtin_expect(__temp40__,0))) {
    __assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/RegionController.cpp",62,"old_size == new_size");
  }
  else {
    (void )0;
  }
}

void RegionController::handle_aircraft_reverse(struct tw_lp *lp)
{
  __restore__(m_q,lp);
}
