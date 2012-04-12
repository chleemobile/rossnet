#ifndef __LOCAL_TRAFFIC_CONTROLLER__
#define __LOCAL_TRAFFIC_CONTROLLER__

#define NUMBER_OF_RUNWAY_LARGE_AIRPORT 20
#define NUMBER_OF_RUNWAY_MEDIUM_AIRPORT 10
#define NUMBER_OF_RUNWAY_SMALL_AIRPORT 5

class LocalTrafficController
{
private:
    int m_num_of_runway;
    int m_num_of_runway_in_use;
    
public:
    enum AirportType
    {
        Small,
        Medium,
        Large
    };
    
    LocalTrafficController(AirportType type);
    ~LocalTrafficController();

    bool dep_req();
    bool landing_req();
    bool arrival_req();
    
    void free_runway();
    
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