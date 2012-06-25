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
