#include "Controller.hpp"
#include "assert.h"

Controller::Controller()
{
}

Controller::Controller(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;
	dummy_test = 0;
}

Controller::~Controller()
{

}

/*
void Controller::handle_incoming(tw_lp *lp)
{
	assert(false);
	cout<<"CTR handle"<<endl;
}

void Controller::handle_outgoing(tw_lp *lp)
{
	assert(false);
	cout<<"CTR handle"<<endl;
}


void Controller::handle_aircraft(tw_lp *lp)
{
	assert(false);
}
*/
