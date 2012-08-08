#include "Controller.hpp"
#include "assert.h"
#include "ross.h" 
#include "rctypes.h" 
#include "backstroke/stl_fwd_rvs_funcs.h" 

Controller::Controller()
{
  (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/air_traffic/Controller.cpp",6,"false") : ((void )0);
}
/*m_counter(in_num_aircraft),*/

Controller::Controller(int in_max_capacity,int in_num_aircraft) : m_max_capacity(in_max_capacity), m_current_capacity(0)
{
}

Controller::~Controller()
{
}

Aircraft Controller::get_aircraft(struct tw_lp *lp) const
{
  class Aircraft ret((this) -> m_q. top ());
  return (ret);;
}

void Controller::remove_aircraft(struct tw_lp *lp)
{
  (this) -> m_q. pop ();
}

void Controller::remove_aircraft_forward(struct tw_lp *lp)
{
  bs_priority_queue_pop_forward((this) -> m_q,lp);
}

void Controller::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_priority_queue_pop_reverse((this) -> m_q,lp);
}

void Controller::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push (aircraft);
}

void Controller::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_priority_queue_push_forward((this) -> m_q,aircraft,lp);
}

void Controller::add_aircraft_reverse(struct tw_lp *lp)
{
  bs_priority_queue_push_reverse((this) -> m_q,lp);
}
