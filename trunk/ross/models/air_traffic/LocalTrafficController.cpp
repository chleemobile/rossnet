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
	m_current_capacity--;
}

void LocalTrafficController::handle_aircraft(tw_lp *lp)
{
	int i=0;
	int size = m_q.size();
	while(i<size)
	{
		m_q.at(i).m_cdelay++;
		i++;
	}
}


