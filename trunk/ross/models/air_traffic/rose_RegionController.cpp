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
  int i = 0;
  int size = ((this) -> m_q. size ());
  while(i < size){
    (this) -> m_q. at (i).Aircraft::m_process_time -= (this) -> m_q. at (i).Aircraft::m_speed;
    (this) -> m_q. at (i).Aircraft::m_remaining_dist -= (this) -> m_q. at (i).Aircraft::m_speed;
    (this) -> m_q. at (i).Aircraft::m_cdelay++;
    i++;
  }
}

void RegionController::handle_aircraft_forward(struct tw_lp *lp)
{
  int i = 0;
  int size = ((this) -> m_q. size ());
  __store__(m_q,lp);
  while(i < size){
    (this) -> m_q. at (i).Aircraft::m_process_time -= (this) -> m_q. at (i).Aircraft::m_speed;
    (this) -> m_q. at (i).Aircraft::m_remaining_dist -= (this) -> m_q. at (i).Aircraft::m_speed;
    (this) -> m_q. at (i).Aircraft::m_cdelay++;
    i++;
  }
}

void RegionController::handle_aircraft_reverse(struct ::tw_lp *lp)
{
  __restore__(m_q,lp);
}
