#include "RegionController.hpp"

RegionController::RegionController(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;
	dummy_test = 1.2345;
}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(tw_lp *lp)
{
	//cout<<"RC handle"<<endl;
	m_current_capacity++;
	//dm_num_aircrafts++;
}

void RegionController::handle_outgoing(tw_lp *lp)
{
	//cout<<"RC handle"<<endl;
	m_current_capacity--;
	//dm_num_aircrafts++;
}

void RegionController::handle_aircraft(tw_lp *lp)
{
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
}
