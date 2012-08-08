#include "LocalTrafficController.hpp"

LocalTrafficController::LocalTrafficController(int in_max_capacity, int in_num_airport):Controller(in_max_capacity, in_num_airport)
{

}

LocalTrafficController::~LocalTrafficController()
{

}

void LocalTrafficController::handle_incoming(tw_lp *lp)
{
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

}

Aircraft LocalTrafficController::get_aircraft(tw_lp *lp)
{
	Aircraft ret = m_q.top();
	return ret;
}


void LocalTrafficController::remove_aircraft(tw_lp *lp)
{
	m_q.pop();
}

void LocalTrafficController::add_aircraft(Aircraft aircraft, tw_lp *lp)
{
	m_q.push(aircraft);
}

