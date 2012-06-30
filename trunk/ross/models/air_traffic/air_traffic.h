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

#include "Controller.hpp"
#include "RegionController.hpp"
#include "LocalTrafficController.hpp"

//using std::max;
//using namespace std;

typedef struct airport_state airport_state;
typedef struct air_traffic_message air_traffic_message;

enum air_traffic_event_t
{
	DEP_REQ,
	DEP,
	TAXI_OUT,
	TAKE_OFF,
	TRANSIT_REQ,
	TRANSIT,
	ON_THE_AIR,
	LANDING_REQ,
	LANDING,
	TAXI_IN,
	ARRIVAL 
};

typedef enum air_traffic_event_t air_traffic_event_t;

struct airport_state
{
	int 	rn;

	Controller *controller;
	vector<int> *counter;

	double  delay_airport_dep;
	double  delay_airport_land;
	
	int     cdelay_airport_dep;
	int 	cdelay_airport_land;


	double  delay_region;
	int     cdelay_region;

	int     max_queue_size_airport;
	int     max_queue_size_region;


	/*
	   Region Controller State Variable
	 */


	int transit_req_accepted;
	int transit_req_rejected;
	int transit_processed;

	/*
	   Traffic Controller State Variable
	 */

	int landing_processed;
	int landing_req_accepted;
	int landing_req_rejected;

	int dep_processed;
	int dep_req_accepted;
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

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;
static int p_run = 1;
static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 4096;
static int       planes_per_airport = NUMBER_OF_PLANES_PER_AIRPORT;


static int total_transit_req_accepted = 0;
static int total_transit_req_rejected = 0;
static int total_transit_processed = 0;

static int total_dep_req_accepted= 0;
static int total_dep_req_rejected = 0;
static int total_dep_processed= 0;

static int total_landing_req_accepted = 0;
static int total_landing_req_rejected = 0;
static int total_landing_processed = 0;

static int max_queue_size_airport = 0;
static int max_queue_size_region = 0;

static int total_cdelay_airport_dep = 0;
static int total_cdelay_airport_land = 0;
static int total_cdelay_region = 0;

static double total_delay_airport_dep = 0;
static double total_delay_airport_land = 0;
static double total_delay_region = 0;



#endif
