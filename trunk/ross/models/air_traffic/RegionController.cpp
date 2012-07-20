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
	
}

