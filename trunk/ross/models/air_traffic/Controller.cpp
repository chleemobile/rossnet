#include "Controller.hpp"
#include "assert.h"

Controller::Controller()
{
	assert(false);
}

Controller::Controller(int in_max_capacity, int in_num_aircraft):/*m_counter(in_num_aircraft),*/ m_max_capacity(in_max_capacity), m_current_capacity(0)
{
}

Controller::~Controller()
{

}


Aircraft Controller::get_aircraft(tw_lp *lp) const
{
	Aircraft ret = m_q.top();
	return ret;
}


void Controller::remove_aircraft(tw_lp *lp)
{
	m_q.pop();
}

void Controller::add_aircraft(Aircraft aircraft, tw_lp *lp)
{
	m_q.push(aircraft);
}


