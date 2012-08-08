#include "RegionController.hpp"
#include "ross.h" 
#include "rctypes.h" 
#include "backstroke/stl_fwd_rvs_funcs.h" 

RegionController::RegionController(int in_max_capacity,int in_num_aircraft) : Controller(in_max_capacity,in_num_aircraft)
{
}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(struct tw_lp *lp)
{
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
}

void RegionController::handle_incoming_forward(struct tw_lp *lp)
{
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
}

void RegionController::handle_incoming_reverse(struct ::tw_lp *lp)
{
  --m_aircraft_processed;
  --m_current_capacity;
}

void RegionController::handle_outgoing(struct tw_lp *lp)
{
  (this) -> m_current_capacity--;
}

void RegionController::handle_outgoing_forward(struct tw_lp *lp)
{
  (this) -> m_current_capacity--;
}

void RegionController::handle_outgoing_reverse(struct ::tw_lp *lp)
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
}

void RegionController::handle_aircraft_reverse(struct ::tw_lp *lp)
{
  __restore__(m_q,lp);
}
