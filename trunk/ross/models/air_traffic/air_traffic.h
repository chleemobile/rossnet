#ifndef INC_airport_h
#define INC_airport_h

class Aircraft;
class LocalTrafficController;
class RegionTrafficController;

#include <ross.h>
#include "iostream"
#include <stack>


#define DEBUG 0
#define NUMBER_OF_LP 348
#define NUMBER_OF_REGION_CONTROLLER 20

using std::max;
using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

enum air_traffic_event_t
{
    DEP_REQ=1,
    TAXI_OUT,
    TAKE_OFF_REQ,
    TAKE_OFF_REP,
    TAKE_OFF,
    ON_THE_AIR,
    TRANSIT_REQ,
    HAND_OFF,
    LANDING_REQ,
    LANDING,
    ARRIVAL_REQ,
    TAXI_IN,
    ARRIVAL
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
    int rn;
    
    Aircraft                *airplane;
    LocalTrafficController  *traffic_controller;
    RegionTrafficController *region_controller;
};

struct air_traffic_message
{
	air_traffic_event_t	 type;
    
    Aircraft     *airplane;
    int          msg_from;
};

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;

static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 100000;
static int       planes_per_airport = 250;

static tw_stime	 wait_time_avg = 0.0;


#endif
