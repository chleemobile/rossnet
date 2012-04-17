#ifndef __LOCAL_TRAFFIC_CONTROLLER__
#define __LOCAL_TRAFFIC_CONTROLLER__


#define NUMBER_OF_RUNWAY_LARGE_AIRPORT 6
#define NUMBER_OF_RUNWAY_MEDIUM_AIRPORT 3
#define NUMBER_OF_RUNWAY_SMALL_AIRPORT 2
#define NUMBER_OF_RUNWAY_NH_AIRPORT 1

#define MEAN_DEP_PREPARE 10.0
#define MEAN_DEPARTURE 30.0
#define MEAN_LAND 10.0
#define MEAN_DELAY 10.0

typedef double (*rn_f_exp) (int &, double);
typedef long (*rn_f_int) (int &, long, long);

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
        Large,
        NH
    };
    
    LocalTrafficController(AirportType type);
    ~LocalTrafficController();

    bool dep_req();
    bool landing_req();
    bool arrival_req();
    
    void free_runway();
    
    double cal_dep_prep_time(int &seed, rn_f_exp f_ptr);
    double cal_delay(int &seed, rn_f_exp f_ptr);
    
    long cal_taxi_out_time(int &seed, rn_f_int f_ptr);

    int cal_take_off_prep_time();
    int cal_take_off_time();

    int cal_landing_prep_time();
    int cal_landing_time();
    
    int cal_taxi_in_time();

    int cal_arrival_prep_time();
    int cal_arrival_time();
    
};

#endif