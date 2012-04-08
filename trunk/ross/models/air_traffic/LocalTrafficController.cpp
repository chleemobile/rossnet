#include "LocalTrafficController.hpp"

LocalTrafficController::LocalTrafficController()
{
    
}

LocalTrafficController::~LocalTrafficController()
{
    
}

bool LocalTrafficController::dep_req()
{
    return true;
}

bool LocalTrafficController::take_off_req()
{
    return true;
}

bool LocalTrafficController::landing_req()
{
    return true;
}

bool LocalTrafficController::arrival_req()
{
    return true;
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
