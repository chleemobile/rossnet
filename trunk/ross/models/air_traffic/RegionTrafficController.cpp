#include "RegionTrafficController.hpp"

RegionTrafficController::RegionTrafficController()
{
    
}

RegionTrafficController::~RegionTrafficController()
{
    
}

bool RegionTrafficController::take_off_req()
{
    return true;
}

bool RegionTrafficController::transit_req(int from, int to)
{
    return true;
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