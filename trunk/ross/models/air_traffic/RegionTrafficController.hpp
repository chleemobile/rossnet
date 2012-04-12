#ifndef __REGION_TRAFFIC_CONTROLLER__
#define __REGION_TRAFFIC_CONTROLLER__

struct airport_state;

#define AIRCRAFT_CAPACITY_OF_LARGE_REGION 200
#define AIRCRAFT_CAPACITY_OF_MEDIUM_REGION 100
#define AIRCRAFT_CAPACITY_OF_SMALL_REGION 50

class RegionTrafficController
{
private:
    int m_max_capacity;
    int m_num_airplanes;
    
public:
    enum RegionType
    {
        Small,
        Medium,
        Large
    };
    
    RegionTrafficController(RegionType type);
    ~RegionTrafficController();
    
    /*
     set num_airplanes
     */
    bool take_off_req();
    bool transit_req(int from, int to);

    int cal_delay();
    int cal_flight_time();
    int cal_transit_time();
};

#endif