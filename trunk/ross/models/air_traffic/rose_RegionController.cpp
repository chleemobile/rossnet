#include "RegionController.hpp"
#include <ross.h>
#include "rctypes.h" 

RegionController::RegionController(int in_max_capacity,int in_num_aircraft) : Controller(in_max_capacity,in_num_aircraft)
{
}

RegionController::~RegionController()
{
}
/*
void RegionController::handle_incoming(tw_lp *lp)
{
	m_current_capacity++;
	m_req_accepted++;
	m_aircraft_processed++;
}
void RegionController::handle_outgoing(tw_lp *lp)
{
	m_current_capacity--;
}
*/

void RegionController::handle_aircraft(struct tw_lp *lp)
{
//priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > temp_q;
  int old_size = ((this) -> m_in_q. size ());
  while(!(this) -> m_in_q. empty ()){
    class Aircraft temp((this) -> m_in_q. top ());
    (this) -> m_in_q. pop ();
    temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
    temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
    temp.Aircraft::m_cdelay++;
    temp_q. push (temp);
  }
  while(!temp_q. empty ()){
    class Aircraft temp(temp_q. top ());
    temp_q. pop ();
    (this) -> m_in_q. push (temp);
  }
  class Aircraft new_top((this) -> m_in_q. top ());
  int new_size = ((this) -> m_in_q. size ());
//delete temp_q;
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
//priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
  class ::std::priority_queue< Aircraft  , ::std::vector< Aircraft  , ::std::allocator< Aircraft  >  >  , ::std::less< Aircraft  >  > temp_q;
  int old_size = ((this) -> m_in_q. size ());
  __store__(m_in_q,lp);
  while(!(this) -> m_in_q. empty ()){
    class Aircraft temp((this) -> m_in_q. top ());
    (this) -> m_in_q. pop ();
    temp.Aircraft::m_process_time -= temp.Aircraft::m_speed;
    temp.Aircraft::m_remaining_dist -= temp.Aircraft::m_speed;
//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
    temp.Aircraft::m_cdelay++;
    temp_q. push (temp);
  }
  while(!temp_q. empty ()){
    class Aircraft temp(temp_q. top ());
    temp_q. pop ();
    (this) -> m_in_q. push (temp);
  }
  class Aircraft new_top((this) -> m_in_q. top ());
  int new_size = ((this) -> m_in_q. size ());
//delete temp_q;
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
  __restore__(m_in_q,lp);
}
