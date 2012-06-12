#include "LocalTrafficController.hpp"

LocalTrafficController::LocalTrafficController()
{

}

LocalTrafficController::~LocalTrafficController()
{

}

void LocalTrafficController::handle()
{
	cout<<"LTC handle"<<endl;
	m_num_runways_used++;
}
