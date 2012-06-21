#include "Controller.hpp"
#include "assert.h"


Controller::Controller()
{
}

Controller::Controller(int in_max_capacity, int in_num_aircraft)
{
	//m_id;
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;
		
	m_aircraft_processed = 0;
	m_req_accepted = 0;
	m_req_rejected = 0;

	m_max_q_size = 0;
	m_max_counter = 0;
	m_mc_aircraft_id = 0;

	m_delay = 0;
	m_cdelay = 0;

	m_in_q = new priority_queue<Aircraft, vector<Aircraft>, less<Aircraft> >();
	m_counter = new vector<int> (in_num_aircraft);

}

Controller::~Controller()
{
//	delete m_in_q;
//	delete m_counter;
}

/*
void Controller::handle_incoming(tw_lp *lp)
{
	assert(false);
}

void Controller::handle_outgoing(tw_lp *lp)
{
	assert(false);
}
*/

void Controller::handle_aircraft(tw_lp *lp)
{
	assert(false);
}

