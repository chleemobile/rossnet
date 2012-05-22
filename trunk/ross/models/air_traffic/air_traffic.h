#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include "iostream"
#include <stack>
#include <deque>
#include "Graph.hpp"
#include "Constants.hpp"
#include <map>

using std::max;
using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

typedef map<int, int> inner_map;
typedef map<int, inner_map> outer_map;

static outer_map m;

enum air_traffic_event_t
{
    DEP_REQ,
    DEP_DELAY,
	TAXI_OUT,
    TAKE_OFF,
	TRANSIT_REQ,
	ON_THE_AIR,
	TRANSIT_DELAY,
    LANDING_REQ,
	LANDING_DELAY,
	LANDING,
	TAXI_IN,
	ARRIVAL
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
    int rn;
    /*
     Region Controller State Variable
     */
    
    int max_capacity;
    int airplane_in_region;
    
	int transit_req_accepted;
    int transit_req_rejected;
    
    /*
     Traffic Controller State Variable
     */
    int max_runway;
    int runway_in_use;
    
	int landing;
    int landing_req_accepted;
    int landing_req_rejected;

    int dep_req_accepted;
    int dep_req_rejected;
};

struct air_traffic_message
{
	air_traffic_event_t	 type;
    
    int dest_region;
    int dest_airport;
    
    int msg_from;
};

static int total_transit_accepted = 0;
static int total_transit_rejected = 0;

static int total_dep_req_accepted = 0;
static int total_dep_req_rejected = 0;

static int total_landing_req_accepted = 0;
static int total_landing_req_rejected = 0;

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;

static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 100000;
static int       planes_per_airport = NUMBER_OF_PLANES_PER_AIRPORT;

static tw_stime	 wait_time_avg = 0.0;
static Graph *graph;

#endif
