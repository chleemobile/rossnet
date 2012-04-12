#include "assert.h"
#include "LocalTrafficController.hpp"
#include "air_traffic.h"

LocalTrafficController::LocalTrafficController(AirportType type)
{
    if(type == Small)
    {
        m_num_of_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
    }
    else if(type == Medium)
    {
        m_num_of_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
    }
    else if(type == Large)
    {
        m_num_of_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
    }
    else
    {
        cout<<"LocalTrafficController, Invalid Type"<<endl;
        assert(false);
    }
    m_num_of_runway_in_use = 0;
}

LocalTrafficController::~LocalTrafficController()
{
    
}

bool LocalTrafficController::dep_req()
{
    if(m_num_of_runway_in_use < m_num_of_runway)
    {
        m_num_of_runway_in_use++;
        return true;
    }
    return false;
}

bool LocalTrafficController::landing_req()
{
    return true;
}

bool LocalTrafficController::arrival_req()
{
    return true;
}


void LocalTrafficController::free_runway()
{
    m_num_of_runway_in_use--;
    assert(m_num_of_runway_in_use >= 0);
}

int LocalTrafficController::cal_dep_prep_time()
{
    return 1;
}

int LocalTrafficController::cal_delay()
{
    return 1;
}

int LocalTrafficController::cal_taxi_out_time()
{
    return 1;
}

int LocalTrafficController::cal_take_off_prep_time()
{
    return 1;
}

int LocalTrafficController::cal_take_off_time()
{
    return 1;
}


int LocalTrafficController::cal_landing_prep_time()
{
    return 1;
}

int LocalTrafficController::cal_landing_time()
{
    return 1;
}

int LocalTrafficController::cal_taxi_in_time()
{
    return 1;
}

int LocalTrafficController::cal_arrival_prep_time()
{
    return 1;
}

int LocalTrafficController::cal_arrival_time()
{
    return 1;
}
