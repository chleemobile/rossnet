#include "LocalTrafficController.hpp"

LocalTrafficController::LocalTrafficController(int in_max_capacity, int in_num_airport):Controller(in_max_capacity, in_num_airport)
{

}

LocalTrafficController::~LocalTrafficController()
{

}

void LocalTrafficController::handle_incoming(tw_lp *lp)
{
	//cout<<"LTC handle"<<endl;
	m_current_capacity++;
	m_aircraft_processed++;	

}

void LocalTrafficController::handle_outgoing(tw_lp *lp)
{
	//cout<<"LTC handle"<<endl;
	m_current_capacity--;
}

void LocalTrafficController::handle_aircraft(tw_lp *lp)
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

	int i=0;
	int size = m_q.size();

	while(i<size)
	{
		Aircraft aircraft = m_q.front();
		m_q.pop_front();
		aircraft.m_cdelay++;
		m_q.push_back(aircraft);		
		i++;
	}
}

#define CTR_Q_DEBUG 

Aircraft LocalTrafficController::get_aircraft(tw_lp *lp) const
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

	Aircraft ret = m_q.back();
	*/
	Aircraft ret = *max_element(m_q.begin(), m_q.end());
//	cout<<ret.m_process_time<<","<<ret2.m_process_time<<endl;

	return ret;
}


void LocalTrafficController::remove_aircraft(tw_lp *lp)
{
	m_q.erase(max_element(m_q.begin(), m_q.end()));

//	m_q.pop_back();
}

void LocalTrafficController::add_aircraft(Aircraft aircraft, tw_lp *lp)
{
	m_q.push_back(aircraft);
}

	template <class T>
bool LocalTrafficController::sort_predicate(const T &lhs, const T &rhs)
{
	if(lhs.m_process_time == rhs.m_process_time)
		return (lhs.m_id > rhs.m_id);//possible in parallel run
	return (lhs.m_process_time > rhs.m_process_time);	
}


