#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include <stack>
#include <deque>
#include <map>
#include <string>
#include <sstream> 
#include <fstream>
#include <iostream>

#include <stdio.h>

#include "iostream"
#include "Graph.hpp"
#include "Constants.hpp"


//using std::max;
//using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

enum air_traffic_event_t
{
    DEP_REQ, //0
    DEP_DELAY, //1
	TAXI_OUT, //2
    TAKE_OFF, //3
	TRANSIT_REQ, //4
	ON_THE_AIR, //5
	TRANSIT_DELAY, //6
    LANDING_REQ, //7
	LANDING_DELAY, //8
	LANDING, //9
	TAXI_IN, //10
	ARRIVAL //11
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


typedef struct counter_container counter_container;

struct counter_container
{
    int total_event_count;
    int net_event_count;
};

typedef std::map<int, counter_container> inner_map;
typedef std::map<int, inner_map> outer_map;

static outer_map counters;

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
