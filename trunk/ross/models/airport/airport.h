#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include <vector>
#include "iostream"
#include <stack>
#include "Aircraft.hpp"
#include "Controller.hpp"

#define NUMBER_OF_LP 256
#define MEAN_DEPARTURE 30.0
#define MEAN_LAND 10.0

const int MAX_CAPACITY = 1;

using namespace std;

typedef struct airport_state airport_state;
typedef struct airport_message airport_message;

enum airport_event_t
{
	ARRIVAL = 1, 
	DEPARTURE,
	LAND
};

typedef enum airport_event_t airport_event_t;

struct airport_state
{
    
    int rn;
	int max_capacity;
	int current_capacity;

	int dep_processed;
	int dep_queued;

	vector<Aircraft> queue;
};

struct airport_message
{
	airport_event_t	 type;
	Aircraft 		 aircraft;

	tw_stime	 waiting_time;
	tw_stime	 saved_furthest_flight_landing;
    int msg_from;
};

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;


static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 10000;
static int		 planes_per_airport = 10;



#endif
