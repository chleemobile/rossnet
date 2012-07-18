#include "RegionController.hpp"

RegionController::RegionController(int in_max_capacity, int in_num_aircraft):Controller(in_max_capacity, in_num_aircraft)
{

}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(tw_lp *lp)
{
	//cout<<"RC handle"<<endl;
	m_current_capacity++;
	m_aircraft_processed++;

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

	int i=0;
	int size = m_q.size();
	while(i<size)
	{
		m_q.at(i).m_process_time -= m_q.at(i).m_speed;
		m_q.at(i).m_remaining_dist -= m_q.at(i).m_speed;
		m_q.at(i).m_cdelay++;
		
		i++;
	}
	
}

Aircraft RegionController::get_aircraft(tw_lp *lp)
{
	Aircraft ret = *max_element(m_q.begin(), m_q.end());
	return ret;
}

void RegionController::remove_aircraft(tw_lp *lp)
{
	m_q.erase(max_element(m_q.begin(), m_q.end()));
}

void RegionController::add_aircraft(Aircraft aircraft, tw_lp *lp)
{
	m_q.push_back(aircraft);
}

template <class T>
bool RegionController::sort_predicate(const T &lhs, const T &rhs)
{
	if(lhs.m_process_time == rhs.m_process_time)
		return (lhs.m_id > rhs.m_id);//possible in parallel run
	return (lhs.m_process_time > rhs.m_process_time);	
}

