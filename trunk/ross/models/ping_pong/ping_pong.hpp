#ifndef INC_ping_pong_h
#define INC_ping_pong_h

#include <ross.h>
#include <iostream>
#include "assert.h"

using namespace std;

typedef struct ping_pong_state ping_pong_state;
typedef struct ping_pong_message ping_pong_message;

struct ping_pong_state
{
	int	rn;
};

struct ping_pong_message
{
	long int	dummy_data;
};

tw_stime lookahead = 1.0;

//This simulation only works with 2 cores, one LP per each core.
static unsigned int nlp_per_pe = 1;
static int g_start_events = 1;
static int optimistic_memory = 100;


#endif
