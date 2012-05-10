#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include "iostream"
#include <stack>
#include <deque>
#include "Graph.hpp"


#define GRAPH_CSV_FILE_PATH "adjlist_csv.csv"

#define DEBUG 0
#define NUMBER_OF_LP 1024
#define NUMBER_OF_REGION_CONTROLLER 20

#define MEAN_DEQ 5.0
#define MEAN_REQ 5.0
#define MEAN_TAXI 3.0
#define MEAN_FLIGHT 30.0
#define MEAN_REP 2.0
#define MEAN_LAND 10.0
#define MEAN_DELAY 4.0

#define NUMBER_OF_RUNWAY_LARGE_AIRPORT 1
#define NUMBER_OF_RUNWAY_MEDIUM_AIRPORT 1
#define NUMBER_OF_RUNWAY_SMALL_AIRPORT 1
#define NUMBER_OF_RUNWAY_NH_AIRPORT 10

#define AIRCRAFT_CAPACITY_OF_LARGE_REGION 10
#define AIRCRAFT_CAPACITY_OF_MEDIUM_REGION 100
#define AIRCRAFT_CAPACITY_OF_SMALL_REGION 500

#define NUMBER_OF_PLANES_PER_AIRPORT 1

using std::max;
using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

enum air_traffic_event_t
{
    DEP,
    DEP_DELAY,
    TAKE_OFF,
    LAND
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
    int rn;
    int from;
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
    int arrival_req_accepted;
    int arrival_req_rejected;
    int dep_req_accepted;
    int dep_req_rejected;
};

struct air_traffic_message
{
	air_traffic_event_t	 type;
    
    int dest_region;
    int dest_airport;
    int next_region;
    
    int msg_from;
};

static int total_transit_accepted = 0;
static int total_transit_rejected = 0;

static int total_dep_req_accepted = 0;
static int total_dep_req_rejected = 0;

static int total_arrival_req_accepted = 0;
static int total_arrival_req_rejected = 0;

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;

static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 1000000;
static int       planes_per_airport = NUMBER_OF_PLANES_PER_AIRPORT;

static tw_stime	 wait_time_avg = 0.0;
static Graph *graph;

#endif
