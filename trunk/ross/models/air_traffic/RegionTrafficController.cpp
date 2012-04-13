#include "assert.h"
#include "RegionTrafficController.hpp"
#include "air_traffic.h"

RegionTrafficController::RegionTrafficController(RegionType type)
{
    if(type == Small)
    {
        m_max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;
    }
    else if(type == Medium)
    {
        m_max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;
    }
    else if(type == Large)
    {
        m_max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
    }
    else
    {
        cout<<"RegionTrafficController, Invalid Type"<<endl;
        assert(false);
    }
    m_num_airplanes = 0;
}

RegionTrafficController::~RegionTrafficController()
{
    
}

bool RegionTrafficController::take_off_req()
{
    if(m_num_airplanes < m_max_capacity)
    {
        m_num_airplanes++;
        return true;
    }
    return false;
}

bool RegionTrafficController::transit_req()
{
    if(m_num_airplanes < m_max_capacity)
    {
        m_num_airplanes++;
        return true;
    }
    return false;
}

void RegionTrafficController::hand_off()
{
    m_num_airplanes--;
    assert(m_num_airplanes>=0);
}

int RegionTrafficController::cal_delay()
{
    return 1;
}

int RegionTrafficController::cal_flight_time()
{
    return 1;
}

int RegionTrafficController::cal_transit_time()
{
    return 1;
}