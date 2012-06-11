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

tw_peid mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}

void init(airport_state * s, tw_lp * lp)
{
	//BSStack* stack = new BSStack();
	//lp->stack_pointer = stack;

	int i;
	tw_lpid evnt_to;
	tw_stime ts;
	tw_event *e;
	airport_message *m;

	s->rn = lp->gid;
	s->current_capacity = 0;
	s->max_capacity = MAX_CAPACITY;
	s->dep_processed = 0;
	s->dep_queued = 0;
	s->wdelay = 0;
	s->sdelay = 0;

	for(i = 0; i < planes_per_airport; i++)
	{
		evnt_to = lp->gid;
		ts = bs_rand_exponential(s->rn, MEAN_DEPARTURE);

		int aircraft_dest = bs_rand_integer(s->rn, 0, NUMBER_OF_LP-1);		

		e = tw_event_new(evnt_to, ts, lp);

		Aircraft int_aircraft;		
		m = (airport_message*)tw_event_data(e);
		m->type = DEPARTURE;
		m->aircraft = int_aircraft;
		m->aircraft.set_dest(aircraft_dest);

		tw_event_send(e);
	}
}

void event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
	tw_lpid evnt_to;
	tw_stime ts;
	tw_event *e;
	airport_message *m;

	switch(msg->type)
	{

		case ARRIVAL:
			{
				// Schedule a landing in the future	
				//printf("ARRIVAL aircraft %d arrived \n", msg->aircraft.get_id());
				
				evnt_to = lp->gid;
				ts = bs_rand_exponential(s->rn, MEAN_LAND);

				e = tw_event_new(lp->gid, ts, lp);

				m = (airport_message*)tw_event_data(e);
				m->type = LAND;
				m->aircraft = msg->aircraft;
				tw_event_send(e);
				break;
			}

		case DEPARTURE:
			{
				//cout<<"handling "<<msg->aircraft.get_id()<<endl;
			
				msg->aircraft.m_wclock = tw_now(lp);

				s->queue.push_back(msg->aircraft);
	
				if(s->current_capacity < s->max_capacity)
				{
					s->dep_processed++;
					s->current_capacity++;
					
					assert(s->queue.size() > 0);
		
					//printf("DEP handling %d aircraft \n", t_aircraft.get_id());


					Aircraft t_aircraft = s->queue.front();
					s->queue.erase(s->queue.begin());

					s->wdelay += t_aircraft.m_wdelay;
					s->sdelay += t_aircraft.m_sdelay;

					t_aircraft.m_wdelay = 0;
					t_aircraft.m_sdelay = 0;
					t_aircraft.m_wclock = 0;
					
					
					evnt_to = t_aircraft.m_dest;							
					ts = bs_rand_exponential(s->rn, MEAD_FLIGHT);

					e = tw_event_new(evnt_to, ts, lp);

					m = (airport_message*)tw_event_data(e);
					m->type = ARRIVAL;
					m->aircraft = t_aircraft;
					tw_event_send(e);

				}
				else
				{
					s->dep_queued++;

					//cout<<"queuing "<<msg->aircraft.get_id()<<"("<<s->queue.size()<<")"<<endl;

					//vector<Aircraft>::iterator itr = s->queue.begin(); 
					int temp_size = s->queue.size();
					int temp_i = 0;
					while(temp_i < temp_size)
					{
						//s->queue[temp_i].calculate_wdelay(tw_now(lp));
						s->queue[temp_i].m_sdelay++;
						temp_i++;
						//printf("aircraft %d has been waiting %f, %d\n",(*itr).get_id(), (*itr).get_wdelay(), (*itr).get_sdelay());
					}	
					//cout<<""<<endl;
				}

				break;
			}            

		case LAND:
			{
				//printf("LAND aircraft %d arrived \n", msg->aircraft.get_id());
				
				s->current_capacity--;				

				evnt_to = lp->gid;	
				ts = bs_rand_exponential(s->rn, MEAD_FLIGHT);

				e = tw_event_new(evnt_to, ts, lp);

				int aircraft_dest = bs_rand_integer(s->rn, 0, NUMBER_OF_LP-1);		
				Aircraft int_aircraft;		
				m = (airport_message*)tw_event_data(e);
				m->type = DEPARTURE;
				m->aircraft = int_aircraft;
				m->aircraft.m_dest = aircraft_dest;
				
				tw_event_send(e);

				break;
			}

	}
}


void final(airport_state * s, tw_lp * lp)
{
	ttl_wdelay += s->wdelay;
	ttl_sdelay += s->sdelay;
	ttl_dep_processed += s->dep_processed;

	avg_wdelay = ttl_wdelay / ttl_dep_processed;
	avg_sdelay = ttl_sdelay / ttl_dep_processed;
}


tw_lptype airport_lps[] =
{
	{
		(init_f) init,
		(event_f) event_handler,
		(revent_f) event_handler,
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

	tw_run();

	if(tw_ismaster())
	{
		printf("\nAirport Model Statistics:\n");
		printf("\t%-50s %11lld\n", "Number of airports", 
				nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
				planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());
		cout<<"\tAvg wdelay : "<<avg_wdelay<<" in dep queue"<<endl;
		cout<<"\tAvg sdelay : "<<avg_sdelay<<" in dep queue"<<endl;


	}

	tw_end();

	if(MEM_USAGE)
	{
		//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		cout<<memusage<<","<<endl;
	}	

	return 0;
}
