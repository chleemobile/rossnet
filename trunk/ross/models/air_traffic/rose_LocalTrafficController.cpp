#include "LocalTrafficController.hpp"
#include "ross.h" 
#include "rctypes.h" 
#include "backstroke/stl_fwd_rvs_funcs.h" 

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
/*
	   priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
	   Aircraft old_top = m_q.top();
	   int old_size = m_q.size();
	   while(!m_q.empty())
	   {
	   Aircraft temp = m_q.top();
	   m_q.pop();
	   temp.m_cdelay++;
	//cout<<temp.m_cdelay<<endl;
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
	assert(old_top.m_id == new_top.m_id);
	assert(old_size == new_size);
	 */
  int i = 0;
  int size = ((this) -> m_q. size ());
  while(i < size){
    (this) -> m_q. at (i).Aircraft::m_cdelay++;
    i++;
  }
}

void LocalTrafficController::handle_aircraft_forward(struct tw_lp *lp)
{
/*
	   priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *temp_q = new priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> >();
	   Aircraft old_top = m_q.top();
	   int old_size = m_q.size();
	   while(!m_q.empty())
	   {
	   Aircraft temp = m_q.top();
	   m_q.pop();
	   temp.m_cdelay++;
	//cout<<temp.m_cdelay<<endl;
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
	assert(old_top.m_id == new_top.m_id);
	assert(old_size == new_size);
	 */
  int i = 0;
  int size = ((this) -> m_q. size ());
  __store__(m_q,lp);
  while(i < size){
    (this) -> m_q. at (i).Aircraft::m_cdelay++;
    i++;
  }
}

void LocalTrafficController::handle_aircraft_reverse(struct tw_lp *lp)
{
  __restore__(m_q,lp);
}
#define CTR_Q_DEBUG 

Aircraft LocalTrafficController::get_aircraft(struct tw_lp *lp) const
{
/*
#ifdef CTR_Q_DEBUG
	if(m_q.size() == 5 )
	{
		cout<<""<<endl;
		cout<<"before"<<endl;
		for(int i=0; i<m_q.size();i++)
		{
			cout<<m_q[i].m_process_time<<","<<m_q[i].m_id<<","<<m_q[i].m_cdelay<<","<<m_q[i].m_delay<<","<<m_q[i].m_clock<<endl;
		}
	}
#endif
	sort(m_q.begin(), m_q.end());
	//sort(m_q.begin(), m_q.end(), sort_predicate<Aircraft>) // I can't do this. Do you know why? 
#ifdef CTR_Q_DEBUG	
	if(m_q.size() == 5 )
	{
		cout<<""<<endl;
		cout<<"after"<<endl;
		for(int i=0; i<m_q.size();i++)
		{
			cout<<m_q[i].m_process_time<<","<<m_q[i].m_id<<","<<m_q[i].m_cdelay<<","<<m_q[i].m_delay<<","<<m_q[i].m_clock<<endl;
		}
		//assert(false);
	}
#endif
	Aircraft ret = m_q.back();*/
  class Aircraft ret( * (::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > ((this) -> m_q. begin (),(this) -> m_q. end ())));
//	cout<<ret.m_process_time<<","<<ret2.m_process_time<<endl;
  return (ret);
}

Aircraft LocalTrafficController::get_aircraft_forward(struct tw_lp *lp) const
{
/*
#ifdef CTR_Q_DEBUG
	if(m_q.size() == 5 )
	{
		cout<<""<<endl;
		cout<<"before"<<endl;
		for(int i=0; i<m_q.size();i++)
		{
			cout<<m_q[i].m_process_time<<","<<m_q[i].m_id<<","<<m_q[i].m_cdelay<<","<<m_q[i].m_delay<<","<<m_q[i].m_clock<<endl;
		}
	}
#endif
	sort(m_q.begin(), m_q.end());
	//sort(m_q.begin(), m_q.end(), sort_predicate<Aircraft>) // I can't do this. Do you know why? 
#ifdef CTR_Q_DEBUG	
	if(m_q.size() == 5 )
	{
		cout<<""<<endl;
		cout<<"after"<<endl;
		for(int i=0; i<m_q.size();i++)
		{
			cout<<m_q[i].m_process_time<<","<<m_q[i].m_id<<","<<m_q[i].m_cdelay<<","<<m_q[i].m_delay<<","<<m_q[i].m_clock<<endl;
		}
		//assert(false);
	}
#endif
	Aircraft ret = m_q.back();*/
  class Aircraft ret( * (::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::const_iterator  > ((this) -> m_q. begin (),(this) -> m_q. end ())));
//	cout<<ret.m_process_time<<","<<ret2.m_process_time<<endl;
  return (ret);
}

Aircraft LocalTrafficController::get_aircraft_reverse(struct tw_lp *lp) const
{
}

void LocalTrafficController::remove_aircraft(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > ((this) -> m_q. begin (),(this) -> m_q. end ());
  (this) -> m_q. erase (target);
//	m_q.pop_back();
}

void LocalTrafficController::remove_aircraft_forward(struct tw_lp *lp)
{
  ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator target = ::std::max_element< ::std::vector< Aircraft ,::std::allocator< Aircraft  > > ::iterator  > (bs_vector_begin_forward((this) -> m_q,lp),bs_vector_end_forward((this) -> m_q,lp));
  bs_vector_erase_forward((this) -> m_q,target,lp);
//	m_q.pop_back();
}

void LocalTrafficController::remove_aircraft_reverse(struct tw_lp *lp)
{
  bs_vector_erase_reverse((this) -> m_q,lp);
  bs_vector_end_reverse((this) -> m_q,lp);
  bs_vector_begin_reverse((this) -> m_q,lp);
}

void LocalTrafficController::add_aircraft(class Aircraft aircraft,struct tw_lp *lp)
{
  (this) -> m_q. push_back (aircraft);
}

void LocalTrafficController::add_aircraft_forward(class Aircraft aircraft,struct tw_lp *lp)
{
  bs_vector_push_back_forward((this) -> m_q,aircraft,lp);
}

void LocalTrafficController::add_aircraft_reverse(struct tw_lp *lp)
{
  bs_vector_push_back_reverse((this) -> m_q,lp);
}
template < class T >
bool LocalTrafficController :: sort_predicate ( const T & lhs, const T & rhs )
{
 if ( lhs . m_process_time == rhs . m_process_time )
  return ( lhs . m_id > rhs . m_id );
 return ( lhs . m_process_time > rhs . m_process_time );
}
//possible in parallel run
