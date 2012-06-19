#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include <stack>
#include <queue>
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
#include "Aircraft.hpp"

//using std::max;
//using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

enum air_traffic_event_t
{
	DEP_REQ, //0
	DEP,
	TAXI_OUT, //2
	TAKE_OFF, //3
	TRANSIT_REQ, //4
	TRANSIT,
	ON_THE_AIR, //5
	LANDING_REQ, //7
	LANDING, //9
	TAXI_IN, //10
	ARRIVAL //11
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
	int 	rn;
	priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *incoming_queue;
	vector<int> *aircraft_counter;

	double  delay_airport;
	int     cdelay_airport;
	int     max_queue_size_airport;

	double  delay_region;
	int     cdelay_region;
	int     max_queue_size_region;


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

	int dep_processed;
	int dep_req_rejected;
};

struct air_traffic_message
{
	air_traffic_event_t	 type;

	Aircraft aircraft;

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
static Graph *graph;

static int total_transit_accepted = 0;
static int total_transit_rejected = 0;

static int total_dep_processed= 0;
static int total_dep_req_rejected = 0;

static int total_landing_req_accepted = 0;
static int total_landing_req_rejected = 0;

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;
static int p_run = 1;
static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 1000000;
static int       planes_per_airport = NUMBER_OF_PLANES_PER_AIRPORT;

static tw_stime	 wait_time_avg = 0.0;

static int max_queue_size_airport = 0;
static int max_queue_size_region = 0;

static int total_cdelay_airport = 0;
static int total_cdelay_region = 0;

static double total_delay_airport = 0;
static double total_delay_region = 0;

#endif
