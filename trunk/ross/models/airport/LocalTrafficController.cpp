#include "LocalTrafficController.hpp"

LocalTrafficController::LocalTrafficController(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;
}

LocalTrafficController::~LocalTrafficController()
{

}

void LocalTrafficController::handle_incoming(tw_lp *lp)
{
	//cout<<"LTC handle"<<endl;
	m_current_capacity++;

}

void LocalTrafficController::handle_outgoing(tw_lp *lp)
{
	//cout<<"LTC handle"<<endl;
	m_current_capacity--;

}

