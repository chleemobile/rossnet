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
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp44__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp45__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp46__;
  class Aircraft ret(((__temp46__ = ((__temp44__ = (this) -> m_q. begin () , ((__temp45__ = (this) -> m_q. end () , ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > (__temp44__,__temp45__))))) ,  * __temp46__)));
  return (ret);
}

Aircraft Controller::get_aircraft_forward(struct tw_lp *lp) const
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp44__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp45__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp46__;
  class Aircraft ret(((__temp46__ = ((__temp44__ = (this) -> m_q. begin () , ((__temp45__ = (this) -> m_q. end () , ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > (__temp44__,__temp45__))))) ,  * __temp46__)));
  return (ret);
}

Aircraft Controller::get_aircraft_reverse(struct tw_lp *lp) const
{
}

void Controller::remove_aircraft(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp47__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp48__;
  __temp47__ = (this) -> m_q. begin ();
  __temp48__ = (this) -> m_q. end ();
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp47__,__temp48__);
  (this) -> m_q. erase (target);
}

void Controller::remove_aircraft_forward(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp47__;
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp48__;
  __temp47__ = bs_vector_begin_forward((this) -> m_q,lp);
  __temp48__ = bs_vector_end_forward((this) -> m_q,lp);
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp47__,__temp48__);
  bs_vector_erase_forward((this) -> m_q,target,lp);
}

void Controller::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_vector_erase_reverse((this) -> m_q,lp);
  bs_vector_end_reverse((this) -> m_q,lp);
  bs_vector_begin_reverse((this) -> m_q,lp);
}

void Controller::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push_back (aircraft);;
}
