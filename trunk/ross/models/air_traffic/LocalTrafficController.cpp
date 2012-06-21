#include "LocalTrafficController.hpp"
#include "assert.h"

LocalTrafficController::LocalTrafficController(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;

	m_landing_processed = 0;
	m_landing_req_accepted = 0;
	m_landing_req_rejected = 0;

	m_dep_processed = 0;
	m_dep_req_accepted = 0;
	m_dep_req_rejected = 0;

}

LocalTrafficController::~LocalTrafficController()
{

}

void LocalTrafficController::handle_incoming(tw_lp *lp, CTR_OPTION opt)
{
	//cout<<"LTC handle"<<endl;

	if(opt == CTR_DEP)
	{
		m_current_capacity++;
		m_dep_req_accepted++;
		m_dep_processed++;
	}
	else if (opt == CTR_LAND)
	{
		m_current_capacity++;
		m_landing_req_accepted++;
		m_landing_processed++;

	}
	else
	{
		cout<<"invalid handle type"<<endl;
		assert(false);
	}
}

void LocalTrafficController::handle_outgoing(tw_lp *lp, CTR_OPTION opt)
{
	//cout<<"LTC handle"<<endl;

	if(opt == CTR_DEP)
	{
		m_current_capacity--;
	}
	else if (opt == CTR_LAND)
	{

	}
	else
	{
		cout<<"invalid handle type"<<endl;
		assert(false);
	}

}

