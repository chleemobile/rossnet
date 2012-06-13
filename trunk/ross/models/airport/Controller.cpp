#include "Controller.hpp"
#include "assert.h"

Controller::Controller()
{
}

Controller::Controller(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;
}

Controller::~Controller()
{

}

void Controller::handle_incoming()
{
	assert(false);
	cout<<"CTR handle"<<endl;
}

void Controller::handle_outgoing()
{
	assert(false);
	cout<<"CTR handle"<<endl;
}

