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

	//list<Aircraft> *temp_list = new list<Aircraft>();
	int i=0;
	int size = m_q.size();

	while(i<size)
	{
		Aircraft aircraft = m_q.front();
		m_q.pop_front();
		aircraft.m_process_time -= aircraft.m_speed;
		aircraft.m_remaining_dist -= aircraft.m_speed;
		aircraft.m_cdelay++;
		m_q.push_back(aircraft);		
		i++;
	}
	
	/*
	i=0;
	while(i<size)
	{
		Aircraft aircraft = temp_list.pop_front();
		m_q.push_back(aircraft);	
	}
	*/
}

Aircraft RegionController::get_aircraft(tw_lp *lp) const
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

