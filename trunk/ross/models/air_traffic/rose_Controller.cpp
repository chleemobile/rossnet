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
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp42__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp43__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator __temp44__;
  class Aircraft ret(((__temp44__ = ((__temp42__ = (this) -> m_q. begin () , ((__temp43__ = (this) -> m_q. end () , ::std::max_element< ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > (__temp42__,__temp43__))))) ,  * __temp44__)));
  return (ret);;
}

void Controller::remove_aircraft(struct tw_lp *lp)
{
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp45__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp46__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ((__temp45__ = (this) -> m_q. begin () , ((__temp46__ = (this) -> m_q. end () , ::std::max_element< ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp45__,__temp46__)))));
  struct ::std::_Deque_iterator< Aircraft  , Aircraft & , Aircraft * > __temp47__;
  ((__temp47__ = (target) , (this) -> m_q. erase (__temp47__)));
}

void Controller::remove_aircraft_forward(struct tw_lp *lp)
{
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp45__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator __temp46__;
  ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ((__temp45__ = bs_deque_begin_forward((this) -> m_q,lp) , ((__temp46__ = bs_deque_end_forward((this) -> m_q,lp) , ::std::max_element< ::std::deque< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (__temp45__,__temp46__)))));
  struct ::std::_Deque_iterator< Aircraft  , Aircraft & , Aircraft * > __temp47__;
  ((__temp47__ = (target) , bs_deque_erase_forward((this) -> m_q,__temp47__,lp)));
}

void Controller::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_deque_erase_reverse((this) -> m_q,lp);
  bs_deque_end_reverse((this) -> m_q,lp);
  bs_deque_begin_reverse((this) -> m_q,lp);
}

void Controller::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push_back (aircraft);
}

void Controller::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_deque_push_back_forward((this) -> m_q,aircraft,lp);
}

void Controller::add_aircraft_reverse(struct tw_lp *lp)
{
  bs_deque_push_back_reverse((this) -> m_q,lp);
}
