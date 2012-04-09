#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include "iostream"
#include <stack>
#include "LocalTrafficController.hpp"
#include "RegionTrafficController.hpp"

#define DEP_PREPARE 10.0
#define MEAN_DEPARTURE 30.0
#define MEAN_LAND 10.0
#define NUMBER_OF_LP 1024
#define NUMBER_OF_REGION_CONTROLLER 10

using std::max;
using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;
typedef struct aircraft aircraft;

enum air_traffic_event_t
{
    DEP_REQ=1,
    TAXI_OUT,
    TAKE_OFF_REQ,
    TAKE_OFF_REP,
    TAKE_OFF,
    ON_THE_AIR,
    TRANSIT_REQ,
    LANDING_REQ,
    LANDING,
    ARRIVAL_REQ,
    TAXI_IN,
    ARRIVAL
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
    
	int		landings;
	int		planes_in_the_sky;
	int		planes_on_the_ground;

	tw_stime	waiting_time;
	tw_stime	furthest_flight_landing;
    
    int rn;
    aircraft                *airplane;
    LocalTrafficController  *traffic_controller;
    RegionTrafficController *region_controller;
};

struct air_traffic_message
{
	air_traffic_event_t	 type;

	tw_stime	 waiting_time;
	tw_stime	 saved_furthest_flight_landing;
    aircraft     *airplane;
    int          msg_from;
};

struct aircraft
{
    int type;
    int dest_region;
    int dest_airport;
};

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;

static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 1000;
static int       planes_per_airport = 1;

static tw_stime	 wait_time_avg = 0.0;


#endif
