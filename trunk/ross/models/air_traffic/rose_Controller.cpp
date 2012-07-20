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
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp34__;
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp35__;
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp36__;
  class Aircraft ret(((__temp36__ = ((__temp34__ = (this) -> m_q. begin () , ((__temp35__ = (this) -> m_q. end () , ::std::max_element< ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > (__temp34__,__temp35__))))) ,  * __temp36__)));
  return (ret);;
}

void Controller::remove_aircraft(struct tw_lp *lp)
{
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp37__;
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp38__;
  __temp37__ = (this) -> m_q. begin ();
  __temp38__ = (this) -> m_q. end ();
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp37__,__temp38__);
  (this) -> m_q. erase (target);
}

void Controller::remove_aircraft_forward(struct tw_lp *lp)
{
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp37__;
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp38__;
  __temp37__ = bs_list_begin_forward((this) -> m_q,lp);
  __temp38__ = bs_list_end_forward((this) -> m_q,lp);
  ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::list< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp37__,__temp38__);
  bs_list_erase_forward((this) -> m_q,target,lp);
}

void Controller::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_list_erase_reverse((this) -> m_q,lp);
  bs_list_end_reverse((this) -> m_q,lp);
  bs_list_begin_reverse((this) -> m_q,lp);
}

void Controller::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push_back (aircraft);
}

void Controller::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_list_push_back_forward((this) -> m_q,aircraft,lp);
}

void Controller::add_aircraft_reverse(struct tw_lp *lp)
{
  bs_list_push_back_reverse((this) -> m_q,lp);
}
