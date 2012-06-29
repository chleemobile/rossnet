#ifndef INC_simple_sim_h
#define INC_simple_sim_h

#include <ross.h>

typedef struct simple_sim_state simple_sim_state;
typedef struct simple_sim_message simple_sim_message;

struct simple_sim_state
{
	int	rn;
};

struct simple_sim_message
{
	long int	dummy_data;
};

tw_stime lookahead = 1.0;
static unsigned int nlp_per_pe = 1;
static int g_start_events = 1;
static int optimistic_memory = 0;


#endif
