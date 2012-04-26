#include "airport.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>

/*
 airport.c
 Airport simulator
 20011003
 Justin M. LaPre
 
 2008/2/16
 Modified for ROSS 4.0
 David Bauer
 */

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}

void
init(airport_state * s, tw_lp * lp)
{
    //BSStack* stack = new BSStack();
    //lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    airport_message *m;
    
    s->landings = 0;
    s->planes_in_the_sky = 0;
    s->planes_on_the_ground = planes_per_airport;
    s->waiting_time = 0.0;
    s->furthest_flight_landing = 0.0;
    
    for(i = 0; i < planes_per_airport; i++)
    {
        e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, MEAN_DEPARTURE), lp);
        m = (airport_message*)tw_event_data(e);
        m->type = DEPARTURE;
        tw_event_send(e);
    }
}

void
p_init(airport_state * s, tw_lp * lp)
{
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    airport_message *m;
    
    s->landings = 0;
    s->planes_in_the_sky = 0;
    s->planes_on_the_ground = planes_per_airport;
    s->waiting_time = 0.0;
    s->furthest_flight_landing = 0.0;
    
    for(i = 0; i < planes_per_airport; i++)
    {
        e = tw_event_new(lp->gid, bs_rand_exponential2(s->rn, MEAN_DEPARTURE, lp), lp);
        m = (airport_message*)tw_event_data(e);
        m->type = DEPARTURE;
        tw_event_send(e);
    }
}

void
event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
    int rand_result;
    tw_lpid dst_lp;
    tw_stime ts;
    tw_event *e;
    airport_message *m;
    
    switch(msg->type)
    {
            
        case ARRIVAL:
        {
            // Schedule a landing in the future	
            s->planes_in_the_sky++;
            msg->saved_furthest_flight_landing = s->furthest_flight_landing;
            s->furthest_flight_landing = max(s->furthest_flight_landing, tw_now(lp));
            ts = bs_rand_exponential(s->rn, MEAN_LAND);
            e = tw_event_new(lp->gid, ts + s->furthest_flight_landing - tw_now(lp), lp);
            m = (airport_message*)tw_event_data(e);
            m->type = LAND;
            m->msg_from = msg->msg_from;
            assert(msg->msg_from == 5986);

            m->waiting_time = s->furthest_flight_landing - tw_now(lp);
            s->furthest_flight_landing += ts;

            tw_event_send(e);
            break;
        }
            
        case DEPARTURE:
        {
            s->planes_on_the_ground--;
            ts = bs_rand_exponential(s->rn, mean_flight_time);
            rand_result = bs_rand_integer(s->rn, 0, 3);
            dst_lp = 0;
            switch(rand_result)
            {
                case 0:
                    // Fly north
                    if(lp->gid < sqrt_nlp)
                        // Wrap around
                        dst_lp = lp->gid + sqrt_nlp_1 * sqrt_nlp;
                    else
                        dst_lp = lp->gid - sqrt_nlp_1;
                    break;
                case 1:
                    // Fly south
                    if(lp->gid >= sqrt_nlp_1 * sqrt_nlp)
                        // Wrap around
                        dst_lp = lp->gid - sqrt_nlp_1 * sqrt_nlp;
                    else
                        dst_lp = lp->gid + sqrt_nlp_1;
                    break;
                case 2:
                    // Fly east
                    if((lp->gid % sqrt_nlp) == sqrt_nlp_1)
                        // Wrap around
                        dst_lp = lp->gid - sqrt_nlp_1;
                    else
                        dst_lp = lp->gid + 1;
                    break;
                case 3:
                    // Fly west
                    if((lp->gid % sqrt_nlp) == 0)
                        // Wrap around
                        dst_lp = lp->gid + sqrt_nlp_1;
                    else
                        dst_lp = lp->gid - 1;
                    break;
                    
            }
            
            e = tw_event_new(dst_lp, ts, lp);
            m = (airport_message*)tw_event_data(e);
            m->type = ARRIVAL;
            m->msg_from = 5986;
            tw_event_send(e);
            break;
        }            
            
        case LAND:
        {
            s->planes_on_the_ground++;
            s->planes_in_the_sky--;
            s->landings++;
            s->waiting_time += msg->waiting_time;
            
            e = tw_event_new(lp->gid, bs_rand_exponential(s->rn, MEAN_DEPARTURE), lp);
            m = (airport_message*)tw_event_data(e);
            m->type = DEPARTURE;
            m->msg_from = msg->msg_from;
            assert(msg->msg_from == 5986);
            tw_event_send(e);
            break;
        }
            
    }
}


void
fw_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{    
    int rand_result;
    tw_lpid dst_lp;
    tw_stime ts;
    tw_event *e;
    airport_message *m;
    
    switch(msg->type)
    {
        case ARRIVAL:
        {
            // Schedule a landing in the future	
            s->planes_in_the_sky++;
            msg->saved_furthest_flight_landing = s->furthest_flight_landing;
            s->furthest_flight_landing = max(s->furthest_flight_landing, tw_now(lp));
            ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
            e = tw_event_new(lp->gid, ts + s->furthest_flight_landing - tw_now(lp), lp);
            m = (airport_message*)tw_event_data(e);
            m->type = LAND;
            m->waiting_time = s->furthest_flight_landing - tw_now(lp);
            m->msg_from = msg->msg_from;
            assert(msg->msg_from == 5986);
            
            s->furthest_flight_landing += ts;
            tw_event_send(e);
            break;
        }
            
        case DEPARTURE:
        {
            s->planes_on_the_ground--;
            ts = bs_rand_exponential2(s->rn, mean_flight_time, lp);
            rand_result = bs_rand_integer2(s->rn, 0, 3, lp);
            dst_lp = 0;
            
            switch(rand_result)
            {
                case 0:
                    // Fly north
                    if(lp->gid < sqrt_nlp)
                        // Wrap around
                        dst_lp = lp->gid + sqrt_nlp_1 * sqrt_nlp;
                    else
                        dst_lp = lp->gid - sqrt_nlp_1;
                    break;
                case 1:
                    // Fly south
                    if(lp->gid >= sqrt_nlp_1 * sqrt_nlp)
                        // Wrap around
                        dst_lp = lp->gid - sqrt_nlp_1 * sqrt_nlp;
                    else
                        dst_lp = lp->gid + sqrt_nlp_1;
                    break;
                case 2:
                    // Fly east
                    if((lp->gid % sqrt_nlp) == sqrt_nlp_1)
                        // Wrap around
                        dst_lp = lp->gid - sqrt_nlp_1;
                    else
                        dst_lp = lp->gid + 1;
                    break;
                case 3:
                    // Fly west
                    if((lp->gid % sqrt_nlp) == 0)
                        // Wrap around
                        dst_lp = lp->gid + sqrt_nlp_1;
                    else
                        dst_lp = lp->gid - 1;
                    break;
            }
            
            e = tw_event_new(dst_lp, ts, lp);
            m = (airport_message*)tw_event_data(e);
            m->type = ARRIVAL;
            m->msg_from = lp->gid;
            m->type = ARRIVAL;
            m->msg_from = 5986;
            tw_event_send(e);
            break;
        }
            
            
        case LAND:
        {
            s->planes_on_the_ground++;
            s->planes_in_the_sky--;
            s->landings++;
            s->waiting_time += msg->waiting_time;
            
            e = tw_event_new(lp->gid, bs_rand_exponential2(s->rn, MEAN_DEPARTURE, lp), lp);
            m = (airport_message*)tw_event_data(e);
            m->type = DEPARTURE;
            m->msg_from = msg->msg_from;
            assert(msg->msg_from == 5986);
            tw_event_send(e);
            break;
        }
            
    }
}

void
rc_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{    
    switch(msg->type)
    {
        case ARRIVAL:
            s->planes_in_the_sky--;
            s->furthest_flight_landing = msg->saved_furthest_flight_landing;
            bs_rand_rvs(s->rn, lp);
            break;
            
        case DEPARTURE:
            s->planes_on_the_ground++;
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
            break;
            
        case LAND:
            s->planes_on_the_ground--;
            s->planes_in_the_sky++;
            s->landings--;
            s->waiting_time -= msg->waiting_time;
            bs_rand_rvs(s->rn, lp);
            break;
    }
}

void
final(airport_state * s, tw_lp * lp)
{
	wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
}

/*
 Sequential Running
 */


//tw_lptype airport_lps[] =
//{
//	{
//		(init_f) init,
//		(event_f) event_handler,
//		(revent_f) rc_event_handler,
//		(final_f) final,
//		(map_f) mapping,
//		sizeof(airport_state),
//	},
//	{0},
//};


/*
 Parallel Running
 */


tw_lptype airport_lps[] =
{
	{
		(init_f) p_init,
		(event_f) fw_event_handler,
		(revent_f) rc_event_handler,
		(final_f) final,
		(map_f) mapping,
		sizeof(airport_state),
	},
	{0},
};


const tw_optdef app_opt [] =
{
	TWOPT_GROUP("Airport Model"),
	//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
	TWOPT_UINT("nplanes", planes_per_airport, "initial # of planes per airport(events)"),
	TWOPT_STIME("mean", mean_flight_time, "mean flight time for planes"),
	TWOPT_UINT("memory", opt_mem, "optimistic memory"),
	TWOPT_END()
};

int
main(int argc, char **argv, char **env)
{
	int i;
    
	tw_opt_add(app_opt);
	tw_init(&argc, &argv);
    
	nlp_per_pe /= (tw_nnodes() * g_tw_npe);
    
	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(airport_message), 0);
    
	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &airport_lps[0]);
    
    sqrt_nlp = (int)sqrt(nlp);
    sqrt_nlp_1 = sqrt_nlp-1;
	tw_run();
    
	if(tw_ismaster())
	{
		printf("\nAirport Model Statistics:\n");
		printf("\t%-50s %11.4lf\n", "Average Waiting Time", wait_time_avg);
		printf("\t%-50s %11lld\n", "Number of airports", 
               nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
               planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());
	}
    
	tw_end();
	
	if(MEM_USAGE)
	{
        //	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		cout<<memusage<<","<<endl;
	}	
    
	return 0;
}
