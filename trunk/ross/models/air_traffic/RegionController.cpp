#include "RegionController.hpp"

RegionController::RegionController(int in_max_capacity, int in_num_aircraft):Controller(in_max_capacity, in_num_aircraft)
{

}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(tw_lp *lp)
{
	m_current_capacity++;
	m_aircraft_processed++;

}

void RegionController::handle_outgoing(tw_lp *lp)
{
	m_current_capacity--;
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
	
}

