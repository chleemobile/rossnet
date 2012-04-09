#ifndef __LOCAL_TRAFFIC_CONTROLLER__
#define __LOCAL_TRAFFIC_CONTROLLER__

class LocalTrafficController
{
private:
    
public:
    LocalTrafficController();
    ~LocalTrafficController();

    bool dep_req();
    bool landing_req();
    bool arrival_req();
    
    int cal_dep_prep_time();
    int cal_delay();
    
    int cal_taxi_out_time();

    int cal_take_off_prep_time();
    int cal_take_off_time();

    int cal_landing_prep_time();
    int cal_landing_time();
    
    int cal_taxi_in_time();

    int cal_arrival_prep_time();
    int cal_arrival_time();
    
};

#endif