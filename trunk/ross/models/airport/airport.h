#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include "iostream"
#include <stack>

#define MEAN_DEPARTURE 30.0
#define MEAN_LAND 10.0

using std::max;
using namespace std;

typedef struct airport_state airport_state;
typedef struct airport_message airport_message;

enum airport_event_t
{
	ARRIVAL = 1, 
	DEPARTURE,
	LAND,
    DUMMY_REQ,
    DUMMY_REP
};

typedef enum airport_event_t airport_event_t;

struct airport_state
{
	int		landings;
	int		planes_in_the_sky;
	int		planes_on_the_ground;

	tw_stime	waiting_time;
	tw_stime	furthest_flight_landing;
    
    int rn;
};

struct airport_message
{
	airport_event_t	 type;

	tw_stime	 waiting_time;
	tw_stime	 saved_furthest_flight_landing;
    int msg_from;
};

#define NUMBER_OF_LP 1024

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;

static int sqrt_nlp = 0;
static int sqrt_nlp_1 =0;

static tw_stime	 mean_flight_time = 1;
static int       opt_mem = 1000;
static int	 planes_per_airport = 5;

static tw_stime	 wait_time_avg = 0.0;

static stack<int> in_the_sky_stack = stack<int>();
static stack<int> on_the_ground_stack = stack<int>();
static stack<int> landing_stack = stack<int>();
static stack<int> waiting_time_stack = stack<int>();
static stack<int> furthest_flight_landing_stack = stack<int>();

#endif
