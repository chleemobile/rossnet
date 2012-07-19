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
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_forward(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_aircraft_processed++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_reverse(struct ::tw_lp *lp)
{
  --m_aircraft_processed;
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

void RegionController::handle_outgoing_reverse(struct ::tw_lp *lp)
{
  ++m_current_capacity;
}

void RegionController::handle_aircraft(struct tw_lp *lp)
{
/*
	priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
	Aircraft old_top = m_q.top();
	int old_size = m_q.size();
	while(!m_q.empty())
	{
		Aircraft temp = m_q.top();
		m_q.pop();
		temp.m_process_time -= temp.m_speed;
		temp.m_remaining_dist -= temp.m_speed;
		//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
		temp.m_cdelay++;
		temp_q->push(temp);
	}
	while(!temp_q->empty())
	{
		Aircraft temp =temp_q->top();
		temp_q->pop();
		m_q.push(temp);
	}
	Aircraft new_top = m_q.top();
	int new_size = m_q.size();
	delete temp_q;
	//assert(old_top.m_id == new_top.m_id);
	assert(old_size == new_size);
	*/
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
/*
	priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
	Aircraft old_top = m_q.top();
	int old_size = m_q.size();
	while(!m_q.empty())
	{
		Aircraft temp = m_q.top();
		m_q.pop();
		temp.m_process_time -= temp.m_speed;
		temp.m_remaining_dist -= temp.m_speed;
		//if(temp.m_remaining_dist < 0) temp.m_remaining_dist = 0;
		temp.m_cdelay++;
		temp_q->push(temp);
	}
	while(!temp_q->empty())
	{
		Aircraft temp =temp_q->top();
		temp_q->pop();
		m_q.push(temp);
	}
	Aircraft new_top = m_q.top();
	int new_size = m_q.size();
	delete temp_q;
	//assert(old_top.m_id == new_top.m_id);
	assert(old_size == new_size);
	*/
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

Aircraft RegionController::get_aircraft(struct tw_lp *lp) const
{
  class Aircraft ret( * (::std::max_element< ::__gnu_cxx::__normal_iterator< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_pointer  , ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::vector_type  >  > ((this) -> m_q. begin (),(this) -> m_q. end ())));
  return (ret);
}

Aircraft RegionController::get_aircraft_forward(struct tw_lp *lp) const
{
  class Aircraft ret( * (::std::max_element< ::__gnu_cxx::__normal_iterator< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_pointer  , ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::vector_type  >  > ((this) -> m_q. begin (),(this) -> m_q. end ())));
  return (ret);
}

Aircraft RegionController::get_aircraft_reverse(struct ::tw_lp *lp) const
{
}

void RegionController::remove_aircraft(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::__gnu_cxx::__normal_iterator< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::pointer  , ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::vector_type  >  > ((this) -> m_q. begin (),(this) -> m_q. end ());
  (this) -> m_q. erase (target);
}

void RegionController::remove_aircraft_forward(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::__gnu_cxx::__normal_iterator< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::pointer  , ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::vector_type  >  > (bs_vector_begin_forward((this) -> m_q,lp),bs_vector_end_forward((this) -> m_q,lp));
  bs_vector_erase_forward((this) -> m_q,target,lp);
}

void RegionController::remove_aircraft_reverse(struct ::tw_lp *lp)
{
  bs_vector_erase_reverse((this) -> m_q,lp);
  bs_vector_end_reverse((this) -> m_q,lp);
  bs_vector_begin_reverse((this) -> m_q,lp);
}

void RegionController::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push_back (aircraft);
}

void RegionController::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_vector_push_back_forward((this) -> m_q,aircraft,lp);
}

void RegionController::add_aircraft_reverse(struct ::tw_lp *lp)
{
  bs_vector_push_back_reverse((this) -> m_q,lp);
}
template < class T >
bool RegionController :: sort_predicate ( const T & lhs, const T & rhs )
{
 if ( lhs . m_process_time == rhs . m_process_time )
  return ( lhs . m_id > rhs . m_id );
 return ( lhs . m_process_time > rhs . m_process_time );
}
//possible in parallel run
