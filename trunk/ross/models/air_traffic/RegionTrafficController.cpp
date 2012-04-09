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