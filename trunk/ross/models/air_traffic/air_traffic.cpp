#include "air_traffic.h"
#include "rctypes.h"
#include <backstroke/rand.h>
#include <math.h>

/*
 Air_Traffic.cpp
 Air Traffic Simulator
 20120406
 Chayong Lee
 
 2008/2/16
 Modified for ROSS 4.0
 David Bauer
 */

/*
 This is a simple version using a graph to run a path find algorithm!
 */
int get_region(int airport);

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}


void
p_init(airport_state * s, tw_lp * lp)
{
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
    
    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
        s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;
		
    }
    else
    {
        s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
        
        s->runway_in_use=0;
        
        s->landing=0;
		
        s->landing_req_accepted=0;
        s->landing_req_rejected=0;
        s->dep_req_accepted=0;
        s->dep_req_rejected=0;
        
        for(i = 0; i < planes_per_airport; i++)
        {
			tw_stime ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
            e = tw_event_new(lp->gid, ts, lp);            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;            
            tw_event_send(e);
        }
        
    }
}

void
fw_event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    tw_stime ts;
    tw_event *e;
    air_traffic_message *m;
    
    switch(msg->type)
    {
			//        *(int *)bf = (int)0;
            
        case DEP_REQ:
        {
			assert(lp->gid > NUMBER_OF_REGION_CONTROLLER-1);
			
			int weather = bs_rand_integer2(s->rn, 0, 3, lp);
			
			int path = 0;
			
            if ((path = (s->runway_in_use < s->max_runway))) 
            {
				__store__(s->runway_in_use, lp);
				__store__(s->dep_req_accepted, lp);
				
                s->runway_in_use++;
                s->dep_req_accepted++;
				
				
				int dest_airport = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
				int dest_region = get_region(dest_airport);
				
				
				//indecies are one off in the dijkstra alogirthm
				//but will print the path after adding 1 to the value
				
				//				deque<int> p = graph->get_shortest_path(source_region, dest_region);
				//                
				//                if (p.size() != 1) 
				//				{
				//					p.pop_front();
				//                    dest_region = p.front();
				//					//cout << dest_region<<endl;
				//                }
				//				else {
				//					assert(source_region == dest_region);
				//				}
				
				
				//				while (!p.empty()) {
				//					cout << p.front()<<",";
				//					p.pop_front( );
				//				}
				//				cout <<""<<endl;
				
				//assert(false);
				
				ts = bs_rand_exponential2(s->rn, MEAN_DEQ, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TAXI_OUT;
                m->dest_region = dest_region;
				m->dest_airport = dest_airport;
				
				tw_event_send(e);
				
				
            }
            else
            {
				__store__(s->dep_req_rejected, lp);
				s->dep_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = DEP_DELAY;
				m->msg_from = lp->gid;
				
				tw_event_send(e);
				
            }
			
			__store__(path,lp);
			
            break;
        }
			
        case DEP_DELAY:
        {
            
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
            tw_event_send(e);
			
            break;
        }
            
		case TAXI_OUT:
		{
			assert(msg->dest_region < NUMBER_OF_REGION_CONTROLLER);
			
			ts = bs_rand_exponential2(s->rn, MEAN_TAXI, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAKE_OFF;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
			break;
		}
			
        case TAKE_OFF:
        {            
			__store__(s->runway_in_use, lp);
            s->runway_in_use--;
			
			
			int src_region = get_region(lp->gid);
			int next_region =-1;
			deque<int> p = graph->get_shortest_path(src_region, msg->dest_region);
			
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			
			ts = bs_rand_exponential2(s->rn, MEAN_TAKE_OFF, lp);
            
            e = tw_event_new(next_region, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
            m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			
            tw_event_send(e);
			
            break;
        }
            
		case TRANSIT_REQ:
		{
			int path = 0;
			
            if ((path = (s->airplane_in_region < s->max_capacity)))
			{
				
				__store__(s->airplane_in_region, lp);
				__store__(s->transit_req_accepted, lp);
				
				s->airplane_in_region++;
				s->transit_req_accepted++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_FLIGHT, lp);
				
				e = tw_event_new(lp->gid, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = ON_THE_AIR;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
			}
			else
			{
				__store__(s->transit_req_rejected, lp);
				s->transit_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = TRANSIT_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
			}
			
            tw_event_send(e);
			
			__store__(path,lp);
			
			break;
		}
			
		case ON_THE_AIR:
		{
			assert(lp->gid < NUMBER_OF_REGION_CONTROLLER);
			
			int next_region = bs_rand_integer2(s->rn, 0, NUMBER_OF_REGION_CONTROLLER-1, lp);
			
			deque<int> p = graph->get_shortest_path(lp->gid, msg->dest_region);
			
			if (p.size() != 1) 
			{
				p.pop_front();
				next_region = p.front();
			}
			else 
			{
				next_region = msg->dest_region;
			}
			
			__store__(s->airplane_in_region, lp);
			s->airplane_in_region--;
			
			int path = 0;
            if ((path = (next_region == msg->dest_region)))
			{
				ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
				e = tw_event_new(msg->dest_airport, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = LANDING_REQ;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
			}
			else
			{
				ts = bs_rand_exponential2(s->rn, MEAN_REQ, lp);
				
				e = tw_event_new(next_region, ts, lp);
				
				m = (air_traffic_message*)tw_event_data(e);
				m->type = TRANSIT_REQ;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
			}
			
			tw_event_send(e);
			
			__store__(path,lp);
			
			break;
		}
			
		case TRANSIT_DELAY:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TRANSIT_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
        case LANDING_REQ:
        {
			int weather = bs_rand_integer2(s->rn, 0, 3, lp);
			
			int path = 0;
			
            if ((path = (s->runway_in_use < s->max_runway))) 
            {
				__store__(s->runway_in_use, lp);
				__store__(s->landing_req_accepted, lp);
				
                s->runway_in_use++;
                s->landing_req_accepted++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
            }
            else
            {
				__store__(s->landing_req_rejected, lp);
				
				s->landing_req_rejected++;
				
				ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
				ts += weather;
				
                e = tw_event_new(lp->gid, ts, lp);
				
                m = (air_traffic_message*)tw_event_data(e);
                m->type = LANDING_DELAY;
				m->dest_region = msg->dest_region;
				m->dest_airport = msg->dest_airport;
				m->msg_from = lp->gid;
				
				
            }
			
			tw_event_send(e);
			
			__store__(path,lp);
			
            break;
        }
			
		case LANDING_DELAY:
		{	
			ts = bs_rand_exponential2(s->rn, MEAN_DELAY, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = LANDING_REQ;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case LANDING:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_TAXI, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = TAXI_IN;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}	
			
		case TAXI_IN:
		{
			ts = bs_rand_exponential2(s->rn, MEAN_ARRIVAL, lp);
			
            e = tw_event_new(lp->gid, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = ARRIVAL;
			m->dest_region = msg->dest_region;
			m->dest_airport = msg->dest_airport;
			m->msg_from = lp->gid;
			
            tw_event_send(e);
			
			break;
		}
			
		case ARRIVAL:
		{
			__store__(s->runway_in_use, lp);
			s->runway_in_use--;
			
			int dest = bs_rand_integer2(s->rn, NUMBER_OF_REGION_CONTROLLER, NUMBER_OF_LP-1, lp);
			
			ts = bs_rand_exponential2(s->rn, MEAN_LAND, lp);
			
            e = tw_event_new(dest, ts, lp);
			
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;
			
            tw_event_send(e);
            
            break;
		}
			
			
    }
}


void
rc_event_handler(airport_state * s, tw_bf * bf, air_traffic_message * msg, tw_lp * lp)
{    
    switch(msg->type)
    {
        case DEP_REQ:
        {
			//cout<<"RE-DEP_REQ"<<endl;
			
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path) {
				
				bs_rand_rvs(s->rn, lp);
				bs_rand_rvs(s->rn, lp);
				
				__restore__(s->dep_req_accepted, lp);
				__restore__(s->runway_in_use, lp);
				
			}
			else {
				
				bs_rand_rvs(s->rn, lp);
				__restore__(s->dep_req_rejected, lp);
			}
			
			bs_rand_rvs(s->rn, lp);
			
            break;
        }
            
        case DEP_DELAY:
        {
			//cout<<"RE-DEP_DELAY"<<endl;
			
            bs_rand_rvs(s->rn, lp);
            
            break;
        }
			
        case TAXI_OUT:
        {
			//cout<<"RE-TAXI_OUT"<<endl;
			
            bs_rand_rvs(s->rn, lp);
            
            break;
        }
            
        case TAKE_OFF:
        {
			//cout<<"RE-TAKE_OFF"<<endl;
			
			bs_rand_rvs(s->rn, lp);
			__restore__(s->runway_in_use, lp);
			
            break;
        }  
            
        case TRANSIT_REQ:
        {
			//cout<<"RE-TRANSIT_REQ"<<endl;
			
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path) 
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->transit_req_accepted, lp);
				__restore__(s->airplane_in_region, lp);
			}
			else 
			{
				bs_rand_rvs(s->rn, lp);
				
				__restore__(s->transit_req_rejected, lp);
			}
			
            break;
        }
            
		case ON_THE_AIR:
		{
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path)
			{
				bs_rand_rvs(s->rn, lp);
				
			}
			else
			{
				bs_rand_rvs(s->rn, lp);
				
			}
			
			__restore__(s->airplane_in_region, lp);
			bs_rand_rvs(s->rn, lp);
			
            break;
		}
			
        case TRANSIT_DELAY:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case LANDING_REQ:
		{
			int path = -1;
			__restore__(path, lp);
			assert(path >= 0);
			
			if (path)
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->landing_req_accepted, lp);
				__restore__(s->runway_in_use, lp);
				
			}
			else
			{
				bs_rand_rvs(s->rn, lp);
				__restore__(s->landing_req_rejected, lp);
				
			}
			
			bs_rand_rvs(s->rn, lp);
			
            break;
		}
			
		case LANDING_DELAY:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case LANDING:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        } 
			
		case TAXI_IN:
        {			
            bs_rand_rvs(s->rn, lp);
			
            break;
        }
			
		case ARRIVAL:
        {			
            bs_rand_rvs(s->rn, lp);
            bs_rand_rvs(s->rn, lp);
			
			__restore__(s->runway_in_use, lp);
			
            break;
        }
    }
}

void
final(airport_state * s, tw_lp * lp)

{
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
    total_transit_accepted += s->transit_req_accepted;
    total_transit_rejected += s->transit_req_rejected;
    
    total_dep_req_accepted += s->dep_req_accepted;
    total_dep_req_rejected += s->dep_req_rejected;
    
    total_landing_req_accepted += s->landing_req_accepted;
    total_landing_req_rejected += s->landing_req_rejected;
    
	//    cout<<lp->gid<<","<<s->arrival_req_accepted<<endl;
}

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
    
    cout<<"tw_nnode "<<tw_nnodes()<<endl;
    cout<<"g_tw "<<g_tw_npe<<endl;
    
	g_tw_events_per_pe =(planes_per_airport * nlp_per_pe / g_tw_npe) + opt_mem;
	tw_define_lps(nlp_per_pe, sizeof(air_traffic_message), 0);
    
    /*
     init graph
     */
	
	cout<<"buillding a graph"<<endl;
	
    graph = new Graph(20);
    graph->create_graph(GRAPH_CSV_FILE_PATH);
	//graph->print_adjmatrix();
	cout<<"done"<<endl;
    /*
     We have two different LPs
     One represents an airport
     The other one represents Region Controller
     */
	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &airport_lps[0]);
    
	tw_run();
    
	if(tw_ismaster())
	{
		printf("\nAir Traffic Model Statistics:\n");
        
        printf("\t%-50s %11lld\n", "Number of LPs (Airport, Region Controller)", 
               nlp_per_pe * g_tw_npe * tw_nnodes());
		printf("\t%-50s %11lld\n", "Number of planes", 
               planes_per_airport * nlp_per_pe * g_tw_npe * tw_nnodes());
        
        cout<<"\tTotal Transit Accepted : "<<total_transit_accepted<<endl;
        cout<<"\tTotal Transit Rejected : "<<total_transit_rejected<<endl;
        
        cout<<"\tTotal Departure Req Accepted : "<<total_dep_req_accepted<<endl;
        cout<<"\tTotal Departure Req Rejected : "<<total_dep_req_rejected<<endl;
        
        cout<<"\tTotal Arrival Req Accepted : "<<total_landing_req_accepted<<endl;
        cout<<"\tTotal Arrival Req Rejected : "<<total_landing_req_rejected<<endl;
        
        
	}
    
	tw_end();
	
	if(MEM_USAGE)
	{
        //	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		cout<<memusage<<","<<endl;
	}	
    
	return 0;
}

int get_region(int airport)
{
    if(airport >=20 && airport <= 38)
    {
        return 0;
    }
    else if(airport >=39 && airport <= 53)
    {
        return 1;
    }
    else if(airport >=54 && airport <= 72)
    {
        return 2;
    }
    else if(airport >=73 && airport <= 92)
    {
        return 3;
    }
    else if(airport >=93 && airport <= 109)
    {
        return 4;
    }    
    else if(airport >=110 && airport <= 114)
    {
        return 5;
    }
    else if(airport >=115 && airport <= 148)
    {
        return 6;
    }    
    else if(airport >=149 && airport <= 166)
    {
        return 7;
    }    
    else if(airport >=167 && airport <= 180)
    {
        return 8;
    }
    else if(airport >=181 && airport <= 201)
    {
        return 9;
    }
    else if(airport >=202 && airport <= 214)
    {
        return 10;
    }
    else if(airport >=215 && airport <= 226)
    {
        return 11;
    }
    else if(airport >=227 && airport <= 243)
    {
        return 12;
    }    
    else if(airport >=244 && airport <= 254)
    {
        return 13;
    }    
    else if(airport >=255 && airport <= 269)
    {
        return 14;
    }    
    else if(airport >=270 && airport <= 294)
    {
        return 15;
    }    
    else if(airport >=295 && airport <= 302)
    {
        return 16;
    }    
    else if(airport >=303 && airport <= 318)
    {
        return 17;
    }        
    else if(airport >=319 && airport <= 337)
    {
        return 18;
    }    
    else if(airport >=338 && airport <= 347)
    {
        return 19;
    }   
    else
    {
        cout<<"airport region failed"<<endl;
        assert(false);
    }
    
}