#ifndef __REGION_TRAFFIC_CONTROLLER__
#define __REGION_TRAFFIC_CONTROLLER__

class RegionTrafficController
{
private:
    int max_capacity;
    int num_airplanes;
    
public:
    RegionTrafficController();
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