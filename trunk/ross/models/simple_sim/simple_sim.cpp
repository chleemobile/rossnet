/*

simple_sim.cpp
Simsiple Simulation to measure ROSS event overhead.

A sequential run in which an event does nothing more than schedule itself again for time (now +1). 
This should measure the minimum ROSS event overhead in a sequential execution.

5/30/2021
Chayong Lee

 */


#include "simple_sim.hpp"

tw_peid
map_to_pe(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}

void
init(simple_sim_state * s, tw_lp * lp)
{
	for(int i=0; i<g_start_events; i++)
	{
		tw_stime ts;
		tw_event *e;
		simple_sim_message *m;

		ts=1;
		e = tw_event_new(lp->gid, ts, lp);
		m = (simple_sim_message *)tw_event_data(e);
		tw_event_send(e);
	}
}


void
event_handler(simple_sim_state * s, tw_bf * bf, simple_sim_message * in_m, tw_lp * lp)
{
	tw_stime ts;
	tw_event *e;
	simple_sim_message *m;

	ts=1;
	e = tw_event_new(lp->gid, ts, lp);
	m = (simple_sim_message *)tw_event_data(e);
	tw_event_send(e);
}

void
event_handler_rc(simple_sim_state * s, tw_bf * bf, simple_sim_message * in_m, tw_lp * lp)
{
}

void
finish(simple_sim_state * s, tw_lp * lp)
{
}

tw_lptype mylps[] = {
	{(init_f) init,
        (event_f) event_handler,
        (revent_f) event_handler_rc,
        (final_f) finish,
        (map_f) map_to_pe,
        sizeof(simple_sim_state)},
	{0},
};

const tw_optdef app_opt[] =
{
	TWOPT_GROUP("simple_sim Model"),
	TWOPT_UINT("nlp", nlp_per_pe, "number of LPs per processor"),
	TWOPT_STIME("lookahead", lookahead, "lookahead for events"),
	TWOPT_UINT("start-events", g_start_events, "number of initial messages per LP"),
	TWOPT_UINT("memory", optimistic_memory, "additional memory buffers"),
	TWOPT_END()
};

int
main(int argc, char **argv, char **env)
{
	int		 i;
    
	lookahead = 1.0;
	tw_opt_add(app_opt);
	tw_init(&argc, &argv);
    
	if( lookahead > 1.0 )
        tw_error(TW_LOC, "Lookahead > 1.0 .. needs to be less\n");
    
	g_tw_events_per_pe = (nlp_per_pe * g_start_events) + optimistic_memory;
	g_tw_lookahead = lookahead;
    
	tw_define_lps(nlp_per_pe, sizeof(simple_sim_message), 0);
    
	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &mylps[0]);
    
 	if( g_tw_mynode == 0 )
	{
	    printf("=============================================\n");
	    printf("Simple Simd Model Configuration..............\n");
	    printf("   Lookahead..............%lf\n", lookahead);
	    printf("   Start-events...........%u\n", g_start_events);
	    printf("   Memory.................%u\n", optimistic_memory);
	    printf("========================================\n\n");
	}
    
	tw_run();
	tw_end();
    
	return 0;
}


