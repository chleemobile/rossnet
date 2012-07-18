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
		m_q.at(i).m_cdelay++;
		i++;
	}
}

Aircraft LocalTrafficController::get_aircraft(tw_lp *lp)
{
	
	int i=0;
	int size = m_q.size();
	int min_i=-1;
	int min_process_time = 99999999;

	while(i<size)
	{
		if(m_q.at(i).m_process_time < min_process_time)
		{
			min_i = i;
			min_process_time = m_q.at(i).m_process_time;
		}
		i++;
	}
	
	Aircraft ret = m_q.at(min_i);
	return ret;
}


void LocalTrafficController::remove_aircraft(tw_lp *lp)
{
	
	deque<Aircraft> *temp_q = new deque<Aircraft>();

	int i=0;
	int size = m_q.size();
	int min_i=-1;
	int min_process_time = 99999999;

	while(i<size)
	{
		if(m_q.at(i).m_process_time < min_process_time)
		{
			min_i = i;
			min_process_time = m_q.at(i).m_process_time;
		}
		i++;
	}

	i=0;
	while(i<size)
	{
		if(i!=min_i)
			temp_q->push_back(m_q.front());
		m_q.pop_front();
		i++;
	}

	while(!temp_q->empty())
	{
		m_q.push_back(temp_q->front());
		temp_q->pop_front();
	}
	delete temp_q;
}

void LocalTrafficController::add_aircraft(Aircraft aircraft, tw_lp *lp)
{
	m_q.push_back(aircraft);
}

