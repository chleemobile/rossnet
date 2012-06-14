#ifndef INC_airport_h
#define INC_airport_h

#include <ross.h>
#include <vector>
#include <deque>

#include "iostream"
#include <stack>
#include "Aircraft.hpp"
#include "Controller.hpp"
#include "RegionController.hpp"
#include "LocalTrafficController.hpp"


#define NUMBER_OF_LP 1024
#define MEAN_DEPARTURE 30.0
#define MEAD_FLIGHT 20.0
#define MEAN_LAND 10.0

#define MAX_CAPACITY 1

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
	airport_state(): controller(NULL)
	{
	}
	~airport_state(){if(this->controller){delete controller;}}

    int rn;
	int max_capacity;
	int current_capacity;

	int dep_processed;
	int dep_queued;

	//vector<Aircraft> q;
	//deque<Aircraft> q;
	void *q;

	double wdelay;
	int sdelay;

	Controller *controller;
	
};

struct airport_message
{
	airport_event_t	 type;
	Aircraft 		 aircraft;

    int msg_from;
};

static  double  ttl_wdelay = 0;
static  long ttl_sdelay = 0;
static  long ttl_dep_processed = 0;
static  long ttl_dep_queued = 0;

static double avg_wdelay = 0;
static double avg_sdelay = 0;

static int nlp = NUMBER_OF_LP;
static tw_lpid	 nlp_per_pe = NUMBER_OF_LP;


static int       opt_mem = 1000000;
static int		 planes_per_airport = 10;


#endif
