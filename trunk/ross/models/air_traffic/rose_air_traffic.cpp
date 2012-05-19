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
#include "rctypes.h" 
int get_region(int airport);

tw_peid mapping(tw_lpid gid)
{
	return ((tw_peid )gid) / g_tw_nlp;
}

void
init(airport_state * s, tw_lp * lp)
{
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
	s->rn=lp->gid;

    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;			
		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;			
		}
		else {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;			
		}
		
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;
		
    }
    else
    {
		if (lp->gid == 20 ||
            lp->gid == 39 ||
            lp->gid == 73 ||
            lp->gid == 93 ||            
            lp->gid == 115||
            lp->gid == 149||            
            lp->gid == 167||
            lp->gid == 181||            
            lp->gid == 227||            
            lp->gid == 244||                        
            lp->gid == 319||                        
            (lp->gid >= 54 && lp->gid <= 56)   ||
            (lp->gid >= 202 && lp->gid <= 203) ||            
            (lp->gid >= 255 && lp->gid <= 256) ||                        
            (lp->gid >= 270 && lp->gid <= 272) ||    
            (lp->gid >= 295 && lp->gid <= 296) ||                
            (lp->gid >= 303 && lp->gid <= 305) ||                
            (lp->gid >= 338 && lp->gid <= 340) )
        {
            s->max_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
        }
        else if (lp->gid == 21 ||
                 lp->gid == 39 ||
                 lp->gid == 110 ||
                 lp->gid == 116 ||
                 lp->gid == 168 ||
                 lp->gid == 204 ||
                 lp->gid == 320 ||                 
                 (lp->gid >= 40 && lp->gid <= 43)   ||
                 (lp->gid >= 57 && lp->gid <= 59)   ||                 
                 (lp->gid >= 150 && lp->gid <= 152) ||
                 (lp->gid >= 182 && lp->gid <= 185) ||
                 (lp->gid >= 215 && lp->gid <= 216) ||                 
                 (lp->gid >= 228 && lp->gid <= 230) ||                                  
                 (lp->gid >= 245 && lp->gid <= 246) ||                 
                 (lp->gid >= 273 && lp->gid <= 275) ||                                  
                 (lp->gid >= 306 && lp->gid <= 307) ||                                                   
                 (lp->gid >= 341 && lp->gid <= 342))                                                                       
        {
            s->max_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
        }
        else if (lp->gid == 22 ||
                 lp->gid == 44 ||
                 lp->gid == 94 ||
                 lp->gid == 343||            
                 (lp->gid >= 60 && lp->gid <= 62)   ||
                 (lp->gid >= 74 && lp->gid <= 75) ||
                 (lp->gid >= 111 && lp->gid <= 112) ||
                 (lp->gid >= 117 && lp->gid <= 120) ||                                  
                 (lp->gid >= 153 && lp->gid <= 155) ||                                  
                 (lp->gid >= 169 && lp->gid <= 171) ||                 
                 (lp->gid >= 186 && lp->gid <= 188) ||                 
                 (lp->gid >= 205 && lp->gid <= 207) ||
                 (lp->gid >= 217 && lp->gid <= 219) ||                 
                 (lp->gid >= 231 && lp->gid <= 234) ||                 
                 (lp->gid >= 247 && lp->gid <= 250) ||                                  
                 (lp->gid >= 257 && lp->gid <= 261) ||                                  
                 (lp->gid >= 276 && lp->gid <= 281) ||                                                   
                 (lp->gid >= 297 && lp->gid <= 298) ||                                                   
                 (lp->gid >= 308 && lp->gid <= 310) ||                                                   
                 (lp->gid >= 321 && lp->gid <= 327))
        {
            s->max_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
        }
        else
        {
            s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
            
        }
		
        
        s->runway_in_use=0;
        
        s->landing=0;
		
        s->landing_req_accepted=0;
        s->landing_req_rejected=0;
        s->dep_req_accepted=0;
        s->dep_req_rejected=0;
        
        for(i = 0; i < planes_per_airport; i++)
        {
			tw_stime ts = bs_rand_exponential(s->rn, MEAN_DEQ);
            e = tw_event_new(lp->gid, ts, lp);            
            m = (air_traffic_message*)tw_event_data(e);
            m->type = DEP_REQ;            
            tw_event_send(e);
        }
        
    }
}


void event_handler(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
	tw_stime ts;
	tw_event *e;
	air_traffic_message *m;
	{
		int __item_selector__ = (int )(msg -> air_traffic_message::type);
		if (__item_selector__ == DEP_REQ) {
			goto LABEL0;;
		}
		else {
			if (__item_selector__ == DEP_DELAY) {
				goto LABEL1;;
			}
			else {
				if (__item_selector__ == TAXI_OUT) {
					goto LABEL2;;
				}
				else {
					if (__item_selector__ == TAKE_OFF) {
						goto LABEL3;;
					}
					else {
						if (__item_selector__ == TRANSIT_REQ) {
							goto LABEL4;;
						}
						else {
							if (__item_selector__ == ON_THE_AIR) {
								goto LABEL5;;
							}
							else {
								if (__item_selector__ == TRANSIT_DELAY) {
									goto LABEL6;;
								}
								else {
									if (__item_selector__ == LANDING_REQ) {
										goto LABEL7;;
									}
									else {
										if (__item_selector__ == LANDING_DELAY) {
											goto LABEL8;;
										}
										else {
											if (__item_selector__ == LANDING) {
												goto LABEL9;;
											}
											else {
												if (__item_selector__ == TAXI_IN) {
													goto LABEL10;;
												}
												else {
													if (__item_selector__ == ARRIVAL) {
														goto LABEL11;;
													}
													else {;
													};
												};
											};
										};
									};
								};
							};
						};
					};
				};
			};
		}
    LABEL0:
		{
			bool __temp0__;
			__temp0__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
			if ((__builtin_expect(__temp0__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",87,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");;
			}
			else {
				(void )0;;
			};
			int weather = (bs_rand_integer(s -> airport_state::rn,0,3));
			if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
				s -> airport_state::runway_in_use++;
				s -> airport_state::dep_req_accepted++;
				int __temp1__;
				__temp1__ = (NUMBER_OF_LP - 1);
				int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp1__));
				int dest_region = get_region(dest_airport);
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TAXI_OUT;
				m -> air_traffic_message::dest_region = dest_region;
				m -> air_traffic_message::dest_airport = dest_airport;
				tw_event_send(e);;
			}
			else {
				s -> airport_state::dep_req_rejected++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = DEP_DELAY;
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);;
			}
			goto LABEL12;;
		};
    LABEL1:
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = DEP_REQ;
		tw_event_send(e);
		goto LABEL12;;;
    LABEL2:
		{
			bool __temp2__;
			__temp2__ = !((msg -> air_traffic_message::dest_region) < NUMBER_OF_REGION_CONTROLLER);
			if ((__builtin_expect(__temp2__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",150,"msg->dest_region < NUMBER_OF_REGION_CONTROLLER");;
			}
			else {
				(void )0;;
			};
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TAKE_OFF;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL3:
		{
			s -> airport_state::runway_in_use--;
			int src_region = get_region((lp -> tw_lp::gid));
			int next_region = -1;
			class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path (src_region,(msg -> air_traffic_message::dest_region));
			if (p. size () != 1) {
				p. pop_front ();
				next_region = p. front ();;
			}
			else {
				next_region = (msg -> air_traffic_message::dest_region);;
			}
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
			e = tw_event_new(next_region,ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TRANSIT_REQ;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL4:
		if ((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity)) {
			s -> airport_state::airplane_in_region++;
			s -> airport_state::transit_req_accepted++;
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = ON_THE_AIR;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
		}
		else {
			s -> airport_state::transit_req_rejected++;
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TRANSIT_DELAY;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
		}
		tw_event_send(e);
		goto LABEL12;;;
    LABEL5:
		{
			bool __temp3__;
			__temp3__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
			if ((__builtin_expect(__temp3__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",244,"lp->gid < NUMBER_OF_REGION_CONTROLLER");;
			}
			else {
				(void )0;;
			};
			int __temp4__;
			__temp4__ = (NUMBER_OF_REGION_CONTROLLER - 1);
			int next_region = (bs_rand_integer(s -> airport_state::rn,0,__temp4__));
			class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path ((lp -> tw_lp::gid),(msg -> air_traffic_message::dest_region));
			if (p. size () != 1) {
				p. pop_front ();
				next_region = p. front ();;
			}
			else {
				next_region = (msg -> air_traffic_message::dest_region);;
			}
			s -> airport_state::airplane_in_region--;
			if (next_region == (msg -> air_traffic_message::dest_region)) {
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
				e = tw_event_new((msg -> air_traffic_message::dest_airport),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			else {
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_REQ);
				e = tw_event_new(next_region,ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TRANSIT_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL6:
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = TRANSIT_REQ;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL7:
		{
			int weather = (bs_rand_integer(s -> airport_state::rn,0,3));
			if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
				s -> airport_state::runway_in_use++;
				s -> airport_state::landing_req_accepted++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			else {
				s -> airport_state::landing_req_rejected++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING_DELAY;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL8:
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = LANDING_REQ;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL9:
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = TAXI_IN;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL10:
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = ARRIVAL;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL11:
		{
			s -> airport_state::runway_in_use--;
			int __temp5__;
			__temp5__ = (NUMBER_OF_LP - 1);
			int dest = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp5__));
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
			e = tw_event_new(dest,ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = DEP_REQ;
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL12:;;;
	}
	s -> airport_state::rn;
	s -> airport_state::max_capacity;
	s -> airport_state::airplane_in_region;
	s -> airport_state::transit_req_accepted;
	s -> airport_state::transit_req_rejected;
	s -> airport_state::max_runway;
	s -> airport_state::runway_in_use;
	s -> airport_state::landing;
	s -> airport_state::landing_req_accepted;
	s -> airport_state::landing_req_rejected;
	s -> airport_state::dep_req_accepted;
	s -> airport_state::dep_req_rejected;
	bf -> tw_bf::c0;
	bf -> tw_bf::c1;
	bf -> tw_bf::c2;
	bf -> tw_bf::c3;
	bf -> tw_bf::c4;
	bf -> tw_bf::c5;
	bf -> tw_bf::c6;
	bf -> tw_bf::c7;
	bf -> tw_bf::c8;
	bf -> tw_bf::c9;
	bf -> tw_bf::c10;
	bf -> tw_bf::c11;
	bf -> tw_bf::c12;
	bf -> tw_bf::c13;
	bf -> tw_bf::c14;
	bf -> tw_bf::c15;
	bf -> tw_bf::c16;
	bf -> tw_bf::c17;
	bf -> tw_bf::c18;
	bf -> tw_bf::c19;
	bf -> tw_bf::c20;
	bf -> tw_bf::c21;
	bf -> tw_bf::c22;
	bf -> tw_bf::c23;
	bf -> tw_bf::c24;
	bf -> tw_bf::c25;
	bf -> tw_bf::c26;
	bf -> tw_bf::c27;
	bf -> tw_bf::c28;
	bf -> tw_bf::c29;
	bf -> tw_bf::c30;
	bf -> tw_bf::c31;
	msg -> air_traffic_message::type;
	msg -> air_traffic_message::dest_region;
	msg -> air_traffic_message::dest_airport;
	msg -> air_traffic_message::msg_from;
	lp -> tw_lp::id;
	lp -> tw_lp::gid;
	lp -> tw_lp::pe;
	lp -> tw_lp::kp;
	lp -> tw_lp::cur_state;
	lp -> tw_lp::state_qh;
	lp -> tw_lp::type;
	lp -> tw_lp::rng;
	lp -> tw_lp::stack_pointer;
	lp -> tw_lp::current_seed;
	lp -> tw_lp::seed_head;
	lp -> tw_lp::seed_tail;;
}

void event_handler_forward(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
	int __num0 = 0;
	tw_stime ts;
	tw_event *e;
	air_traffic_message *m;
	{
		int __item_selector__ = (int )(msg -> air_traffic_message::type);
		if (__item_selector__ == DEP_REQ) {
			goto LABEL0;;
		}
		else {
			__num0 += 8192;
			if (__item_selector__ == DEP_DELAY) {
				goto LABEL1;;
			}
			else {
				__num0 += 4096;
				if (__item_selector__ == TAXI_OUT) {
					goto LABEL2;;
				}
				else {
					__num0 += 2048;
					if (__item_selector__ == TAKE_OFF) {
						goto LABEL3;;
					}
					else {
						__num0 += 1024;
						if (__item_selector__ == TRANSIT_REQ) {
							goto LABEL4;;
						}
						else {
							__num0 += 512;
							if (__item_selector__ == ON_THE_AIR) {
								goto LABEL5;;
							}
							else {
								__num0 += 256;
								if (__item_selector__ == TRANSIT_DELAY) {
									goto LABEL6;;
								}
								else {
									__num0 += 128;
									if (__item_selector__ == LANDING_REQ) {
										goto LABEL7;;
									}
									else {
										__num0 += 64;
										if (__item_selector__ == LANDING_DELAY) {
											goto LABEL8;;
										}
										else {
											__num0 += 32;
											if (__item_selector__ == LANDING) {
												goto LABEL9;;
											}
											else {
												__num0 += 16;
												if (__item_selector__ == TAXI_IN) {
													goto LABEL10;;
												}
												else {
													__num0 += 8;
													if (__item_selector__ == ARRIVAL) {
														goto LABEL11;;
													}
													else {
														__num0 += 4;;
													};
												};
											};
										};
									};
								};
							};
						};
					};
				};
			};
		}
    LABEL0:
		{
			bool __temp0__;
			__temp0__ = !((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1));
			if ((__builtin_expect(__temp0__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",87,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1");;
			}
			else {
				__num0 += 2;
				(void )0;;
			};
			__store__< int  > (s -> airport_state::rn,lp);
			int weather = (bs_rand_integer(s -> airport_state::rn,0,3));
			if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
				s -> airport_state::runway_in_use++;
				s -> airport_state::dep_req_accepted++;
				int __temp1__;
				__temp1__ = (NUMBER_OF_LP - 1);
				int dest_airport = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp1__));
				int dest_region = get_region(dest_airport);
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DEQ);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TAXI_OUT;
				m -> air_traffic_message::dest_region = dest_region;
				m -> air_traffic_message::dest_airport = dest_airport;
				tw_event_send(e);;
			}
			else {
				__num0 += 1;
				s -> airport_state::dep_req_rejected++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = DEP_DELAY;
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);;
			}
			goto LABEL12;;
		};
    LABEL1:
		__store__< int  > (s -> airport_state::rn,lp);
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = DEP_REQ;
		tw_event_send(e);
		goto LABEL12;;;
    LABEL2:
		{
			bool __temp2__;
			__temp2__ = !((msg -> air_traffic_message::dest_region) < NUMBER_OF_REGION_CONTROLLER);
			if ((__builtin_expect(__temp2__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",150,"msg->dest_region < NUMBER_OF_REGION_CONTROLLER");;
			}
			else {
				__num0 += 1;
				(void )0;;
			};
			__store__< int  > (s -> airport_state::rn,lp);
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TAKE_OFF;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL3:
		{
			s -> airport_state::runway_in_use--;
			int src_region = get_region((lp -> tw_lp::gid));
			int next_region = -1;
			class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path(src_region,(msg -> air_traffic_message::dest_region));
			if (p. size () != 1) {
				p. pop_front ();
				next_region = p. front ();;
			}
			else {
				__num0 += 1;
				next_region = (msg -> air_traffic_message::dest_region);;
			}
			__store__< int  > (s -> airport_state::rn,lp);
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAKE_OFF);
			e = tw_event_new(next_region,ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TRANSIT_REQ;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL4:
		if ((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity)) {
			s -> airport_state::airplane_in_region++;
			s -> airport_state::transit_req_accepted++;
			__store__< int  > (s -> airport_state::rn,lp);
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_FLIGHT);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = ON_THE_AIR;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
		}
		else {
			__num0 += 1;
			s -> airport_state::transit_req_rejected++;
			__store__< int  > (s -> airport_state::rn,lp);
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
			e = tw_event_new((lp -> tw_lp::gid),ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = TRANSIT_DELAY;
			m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
			m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
			m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
		}
		tw_event_send(e);
		goto LABEL12;;;
    LABEL5:
		{
			bool __temp3__;
			__temp3__ = !((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER);
			if ((__builtin_expect(__temp3__,0))) {
				__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",244,"lp->gid < NUMBER_OF_REGION_CONTROLLER");;
			}
			else {
				__num0 += 4;
				(void )0;;
			};
			int __temp4__;
			__temp4__ = (NUMBER_OF_REGION_CONTROLLER - 1);
			__store__< int  > (s -> airport_state::rn,lp);
			int next_region = (bs_rand_integer(s -> airport_state::rn,0,__temp4__));
			class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path((lp -> tw_lp::gid),(msg -> air_traffic_message::dest_region));
			if (p. size () != 1) {
				p. pop_front ();
				next_region = p. front ();;
			}
			else {
				__num0 += 2;
				next_region = (msg -> air_traffic_message::dest_region);;
			}
			s -> airport_state::airplane_in_region--;
			if (next_region == (msg -> air_traffic_message::dest_region)) {
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
				e = tw_event_new((msg -> air_traffic_message::dest_airport),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			else {
				__num0 += 1;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_REQ);
				e = tw_event_new(next_region,ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TRANSIT_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL6:
		__store__< int  > (s -> airport_state::rn,lp);
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = TRANSIT_REQ;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL7:
		{
			__store__< int  > (s -> airport_state::rn,lp);
			int weather = (bs_rand_integer(s -> airport_state::rn,0,3));
			if ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)) {
				s -> airport_state::runway_in_use++;
				s -> airport_state::landing_req_accepted++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			else {
				__num0 += 1;
				s -> airport_state::landing_req_rejected++;
				ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
				ts += weather;
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING_DELAY;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);;
			}
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL8:
		__store__< int  > (s -> airport_state::rn,lp);
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_DELAY);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = LANDING_REQ;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL9:
		__store__< int  > (s -> airport_state::rn,lp);
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_TAXI);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = TAXI_IN;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL10:
		__store__< int  > (s -> airport_state::rn,lp);
		ts = bs_rand_exponential(s -> airport_state::rn,MEAN_ARRIVAL);
		e = tw_event_new((lp -> tw_lp::gid),ts,lp);
		m = ((air_traffic_message *)(tw_event_data(e)));
		m -> air_traffic_message::type = ARRIVAL;
		m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
		m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
		m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
		tw_event_send(e);
		goto LABEL12;;;
    LABEL11:
		{
			s -> airport_state::runway_in_use--;
			int __temp5__;
			__temp5__ = (NUMBER_OF_LP - 1);
			__store__< int  > (s -> airport_state::rn,lp);
			int dest = (bs_rand_integer(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,__temp5__));
			ts = bs_rand_exponential(s -> airport_state::rn,MEAN_LAND);
			e = tw_event_new(dest,ts,lp);
			m = ((air_traffic_message *)(tw_event_data(e)));
			m -> air_traffic_message::type = DEP_REQ;
			tw_event_send(e);
			goto LABEL12;;
		};
    LABEL12:;;;
	}
	s -> airport_state::rn;
	s -> airport_state::max_capacity;
	s -> airport_state::airplane_in_region;
	s -> airport_state::transit_req_accepted;
	s -> airport_state::transit_req_rejected;
	s -> airport_state::max_runway;
	s -> airport_state::runway_in_use;
	s -> airport_state::landing;
	s -> airport_state::landing_req_accepted;
	s -> airport_state::landing_req_rejected;
	s -> airport_state::dep_req_accepted;
	s -> airport_state::dep_req_rejected;
	bf -> tw_bf::c0;
	bf -> tw_bf::c1;
	bf -> tw_bf::c2;
	bf -> tw_bf::c3;
	bf -> tw_bf::c4;
	bf -> tw_bf::c5;
	bf -> tw_bf::c6;
	bf -> tw_bf::c7;
	bf -> tw_bf::c8;
	bf -> tw_bf::c9;
	bf -> tw_bf::c10;
	bf -> tw_bf::c11;
	bf -> tw_bf::c12;
	bf -> tw_bf::c13;
	bf -> tw_bf::c14;
	bf -> tw_bf::c15;
	bf -> tw_bf::c16;
	bf -> tw_bf::c17;
	bf -> tw_bf::c18;
	bf -> tw_bf::c19;
	bf -> tw_bf::c20;
	bf -> tw_bf::c21;
	bf -> tw_bf::c22;
	bf -> tw_bf::c23;
	bf -> tw_bf::c24;
	bf -> tw_bf::c25;
	bf -> tw_bf::c26;
	bf -> tw_bf::c27;
	bf -> tw_bf::c28;
	bf -> tw_bf::c29;
	bf -> tw_bf::c30;
	bf -> tw_bf::c31;
	msg -> air_traffic_message::type;
	msg -> air_traffic_message::dest_region;
	msg -> air_traffic_message::dest_airport;
	msg -> air_traffic_message::msg_from;
	lp -> tw_lp::id;
	lp -> tw_lp::gid;
	lp -> tw_lp::pe;
	lp -> tw_lp::kp;
	lp -> tw_lp::cur_state;
	lp -> tw_lp::state_qh;
	lp -> tw_lp::type;
	lp -> tw_lp::rng;
	lp -> tw_lp::stack_pointer;
	lp -> tw_lp::current_seed;
	lp -> tw_lp::seed_head;
	lp -> tw_lp::seed_tail;;
	__store__< int  > (__num0,lp);
}

void event_handler_reverse(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
	int __num0;
	__restore__(__num0,lp);
	if ((__num0 & 16376) == 16368) {
		__restore__(s -> airport_state::rn,lp);
	}
	else {
		if ((__num0 & 16368) == 16352) {
			__restore__(s -> airport_state::rn,lp);
		}
		else {
			if ((__num0 & 16352) == 16320) {
				__restore__(s -> airport_state::rn,lp);
			}
			else {
				if ((__num0 & 12288) == 8192) {
					__restore__(s -> airport_state::rn,lp);
				}
				else {
					if ((__num0 & 16256) == 16128) {
						__restore__(s -> airport_state::rn,lp);
					}
					else {
						if ((__num0 & 15360) == 14336) {
							__restore__(s -> airport_state::rn,lp);
							//graph ->  get_shortest_path_reverse (src_region,(msg -> air_traffic_message::dest_region));
							++s -> airport_state::runway_in_use;
						}
						else {
							if ((__num0 & 15872) == 15360) {
								if ((__num0 & 15873) == 15360) {
									__restore__(s -> airport_state::rn,lp);
									--s -> airport_state::transit_req_accepted;
									--s -> airport_state::airplane_in_region;
								}
								else {
									__restore__(s -> airport_state::rn,lp);
									--s -> airport_state::transit_req_rejected;
								}
							}
							else {
								if ((__num0 & 16320) == 16256) {
									if ((__num0 & 16321) == 16256) {
										--s -> airport_state::landing_req_accepted;
										--s -> airport_state::runway_in_use;
									}
									else {
										--s -> airport_state::landing_req_rejected;
									}
									__restore__(s -> airport_state::rn,lp);
								}
								else {
									if ((__num0 & 14336) == 12288) {
										__restore__(s -> airport_state::rn,lp);
									}
									else {
										if ((__num0 & 16128) == 15872) {
											++s -> airport_state::airplane_in_region;
											//graph ->  get_shortest_path_reverse ((lp -> tw_lp::gid),(msg -> air_traffic_message::dest_region));
											__restore__(s -> airport_state::rn,lp);
										}
										else {
											if ((__num0 & 8192) == 0 || (__num0 & 16380) == 16380) {
												if ((__num0 & 8193) == 0 || (__num0 & 16381) == 16380) {
													--s -> airport_state::dep_req_accepted;
													--s -> airport_state::runway_in_use;
												}
												else {
													--s -> airport_state::dep_req_rejected;
												}
												__restore__(s -> airport_state::rn,lp);
											}
											else {
												__restore__(s -> airport_state::rn,lp);
												++s -> airport_state::runway_in_use;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
/*******************
 Parallel
 ******************/

void
p_init(airport_state * s, tw_lp * lp)
{
    static int init_seed = lp->gid;
    BSStack* stack = new BSStack();
    lp->stack_pointer = stack;
    
    int i;
    tw_event *e;
    air_traffic_message *m;
	s->rn=lp->gid;

    if(lp->gid <NUMBER_OF_REGION_CONTROLLER)
    {
		if (lp->gid == 4 || lp->gid == 5 || lp->gid == 8 || lp->gid == 11 || lp->gid == 13 || lp->gid == 16 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_SMALL_REGION;			
		}
		else if(lp->gid == 0 || lp->gid == 1 || lp->gid == 3 || lp->gid == 7 || lp->gid == 10 || lp->gid == 12 || lp->gid == 14 || lp->gid == 18 || lp->gid == 19 ) {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_MEDIUM_REGION;			
		}
		else {
			s->max_capacity = AIRCRAFT_CAPACITY_OF_LARGE_REGION;			
		}
		
        s->airplane_in_region = 0;
        
        s->transit_req_accepted = 0;
        s->transit_req_rejected = 0;
		
    }
    else
    {
		if (lp->gid == 20 ||
            lp->gid == 39 ||
            lp->gid == 73 ||
            lp->gid == 93 ||            
            lp->gid == 115||
            lp->gid == 149||            
            lp->gid == 167||
            lp->gid == 181||            
            lp->gid == 227||            
            lp->gid == 244||                        
            lp->gid == 319||                        
            (lp->gid >= 54 && lp->gid <= 56)   ||
            (lp->gid >= 202 && lp->gid <= 203) ||            
            (lp->gid >= 255 && lp->gid <= 256) ||                        
            (lp->gid >= 270 && lp->gid <= 272) ||    
            (lp->gid >= 295 && lp->gid <= 296) ||                
            (lp->gid >= 303 && lp->gid <= 305) ||                
            (lp->gid >= 338 && lp->gid <= 340) )
        {
            s->max_runway = NUMBER_OF_RUNWAY_LARGE_AIRPORT;
        }
        else if (lp->gid == 21 ||
                 lp->gid == 39 ||
                 lp->gid == 110 ||
                 lp->gid == 116 ||
                 lp->gid == 168 ||
                 lp->gid == 204 ||
                 lp->gid == 320 ||                 
                 (lp->gid >= 40 && lp->gid <= 43)   ||
                 (lp->gid >= 57 && lp->gid <= 59)   ||                 
                 (lp->gid >= 150 && lp->gid <= 152) ||
                 (lp->gid >= 182 && lp->gid <= 185) ||
                 (lp->gid >= 215 && lp->gid <= 216) ||                 
                 (lp->gid >= 228 && lp->gid <= 230) ||                                  
                 (lp->gid >= 245 && lp->gid <= 246) ||                 
                 (lp->gid >= 273 && lp->gid <= 275) ||                                  
                 (lp->gid >= 306 && lp->gid <= 307) ||                                                   
                 (lp->gid >= 341 && lp->gid <= 342))                                                                       
        {
            s->max_runway = NUMBER_OF_RUNWAY_MEDIUM_AIRPORT;
        }
        else if (lp->gid == 22 ||
                 lp->gid == 44 ||
                 lp->gid == 94 ||
                 lp->gid == 343||            
                 (lp->gid >= 60 && lp->gid <= 62)   ||
                 (lp->gid >= 74 && lp->gid <= 75) ||
                 (lp->gid >= 111 && lp->gid <= 112) ||
                 (lp->gid >= 117 && lp->gid <= 120) ||                                  
                 (lp->gid >= 153 && lp->gid <= 155) ||                                  
                 (lp->gid >= 169 && lp->gid <= 171) ||                 
                 (lp->gid >= 186 && lp->gid <= 188) ||                 
                 (lp->gid >= 205 && lp->gid <= 207) ||
                 (lp->gid >= 217 && lp->gid <= 219) ||                 
                 (lp->gid >= 231 && lp->gid <= 234) ||                 
                 (lp->gid >= 247 && lp->gid <= 250) ||                                  
                 (lp->gid >= 257 && lp->gid <= 261) ||                                  
                 (lp->gid >= 276 && lp->gid <= 281) ||                                                   
                 (lp->gid >= 297 && lp->gid <= 298) ||                                                   
                 (lp->gid >= 308 && lp->gid <= 310) ||                                                   
                 (lp->gid >= 321 && lp->gid <= 327))
        {
            s->max_runway = NUMBER_OF_RUNWAY_SMALL_AIRPORT;
        }
        else
        {
            s->max_runway = NUMBER_OF_RUNWAY_NH_AIRPORT;
            
        }
		
        
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

void fw_event_handler(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
	tw_stime ts;
	tw_event *e;
	air_traffic_message *m;
	switch((msg -> air_traffic_message::type)){
			//        *(int *)bf = (int)0;
		case DEP_REQ:
		{
			{
				(__builtin_expect((!((lp -> tw_lp::gid) > (NUMBER_OF_REGION_CONTROLLER - 1))),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",490,"lp->gid > NUMBER_OF_REGION_CONTROLLER-1") : ((void )0);
				int weather = (bs_rand_integer2(s -> airport_state::rn,0,3,lp));
				int path = 0;
				if ((path = ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)))) {
					__store__< int  > ((s -> airport_state::runway_in_use),lp);
					__store__< int  > ((s -> airport_state::dep_req_accepted),lp);
					s -> airport_state::runway_in_use++;
					s -> airport_state::dep_req_accepted++;
					int dest_airport = (bs_rand_integer2(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,(NUMBER_OF_LP - 1),lp));
					int dest_region = get_region(dest_airport);
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DEQ,lp);
					ts += weather;
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = TAXI_OUT;
					m -> air_traffic_message::dest_region = dest_region;
					m -> air_traffic_message::dest_airport = dest_airport;
					tw_event_send(e);
				}
				else {
					__store__< int  > ((s -> airport_state::dep_req_rejected),lp);
					s -> airport_state::dep_req_rejected++;
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
					ts += weather;
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = DEP_DELAY;
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
					tw_event_send(e);
				}
				__store__< int  > (path,lp);
				break; 
			}
		}
		case DEP_DELAY:
		{
			{
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = DEP_REQ;
				tw_event_send(e);
				break; 
			}
		}
		case TAXI_OUT:
		{
			{
				(__builtin_expect((!((msg -> air_traffic_message::dest_region) < NUMBER_OF_REGION_CONTROLLER)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",561,"msg->dest_region < NUMBER_OF_REGION_CONTROLLER") : ((void )0);
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_TAXI,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TAKE_OFF;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				tw_event_send(e);
				break; 
			}
		}
		case TAKE_OFF:
		{
			{
				__store__< int  > ((s -> airport_state::runway_in_use),lp);
				s -> airport_state::runway_in_use--;
				int src_region = get_region((lp -> tw_lp::gid));
				int next_region = -1;
				class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path (src_region,(msg -> air_traffic_message::dest_region));
				if (p. size () != 1) {
					p. pop_front ();
					next_region = p. front ();
				}
				else {
					next_region = (msg -> air_traffic_message::dest_region);
				}
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_TAKE_OFF,lp);
				e = tw_event_new(next_region,ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TRANSIT_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				tw_event_send(e);
				break; 
			}
		}
		case TRANSIT_REQ:
		{
			{
				int path = 0;
				if ((path = ((s -> airport_state::airplane_in_region) < (s -> airport_state::max_capacity)))) {
					__store__< int  > ((s -> airport_state::airplane_in_region),lp);
					__store__< int  > ((s -> airport_state::transit_req_accepted),lp);
					s -> airport_state::airplane_in_region++;
					s -> airport_state::transit_req_accepted++;
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_FLIGHT,lp);
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = ON_THE_AIR;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				else {
					__store__< int  > ((s -> airport_state::transit_req_rejected),lp);
					s -> airport_state::transit_req_rejected++;
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = TRANSIT_DELAY;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				tw_event_send(e);
				__store__< int  > (path,lp);
				break; 
			}
		}
		case ON_THE_AIR:
		{
			{
				(__builtin_expect((!((lp -> tw_lp::gid) < NUMBER_OF_REGION_CONTROLLER)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",663,"lp->gid < NUMBER_OF_REGION_CONTROLLER") : ((void )0);
				int next_region = (bs_rand_integer2(s -> airport_state::rn,0,(NUMBER_OF_REGION_CONTROLLER - 1),lp));
				class std::deque< int  , std::allocator< int  >  > p = graph ->  get_shortest_path ((lp -> tw_lp::gid),(msg -> air_traffic_message::dest_region));
				if (p. size () != 1) {
					p. pop_front ();
					next_region = p. front ();
				}
				else {
					next_region = (msg -> air_traffic_message::dest_region);
				}
				__store__< int  > ((s -> airport_state::airplane_in_region),lp);
				s -> airport_state::airplane_in_region--;
				int path = 0;
				if ((path = (next_region == (msg -> air_traffic_message::dest_region)))) {
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
					e = tw_event_new((msg -> air_traffic_message::dest_airport),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = LANDING_REQ;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				else {
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_REQ,lp);
					e = tw_event_new(next_region,ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = TRANSIT_REQ;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				tw_event_send(e);
				__store__< int  > (path,lp);
				break; 
			}
		}
		case TRANSIT_DELAY:
		{
			{
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TRANSIT_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);
				break; 
			}
		}
		case LANDING_REQ:
		{
			{
				int weather = (bs_rand_integer2(s -> airport_state::rn,0,3,lp));
				int path = 0;
				if ((path = ((s -> airport_state::runway_in_use) < (s -> airport_state::max_runway)))) {
					__store__< int  > ((s -> airport_state::runway_in_use),lp);
					__store__< int  > ((s -> airport_state::landing_req_accepted),lp);
					s -> airport_state::runway_in_use++;
					s -> airport_state::landing_req_accepted++;
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
					ts += weather;
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = LANDING;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				else {
					__store__< int  > ((s -> airport_state::landing_req_rejected),lp);
					s -> airport_state::landing_req_rejected++;
					ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
					ts += weather;
					e = tw_event_new((lp -> tw_lp::gid),ts,lp);
					m = ((air_traffic_message *)(tw_event_data(e)));
					m -> air_traffic_message::type = LANDING_DELAY;
					m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
					m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
					m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				}
				tw_event_send(e);
				__store__< int  > (path,lp);
				break; 
			}
		}
		case LANDING_DELAY:
		{
			{
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_DELAY,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = LANDING_REQ;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);
				break; 
			}
		}
		case LANDING:
		{
			{
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_TAXI,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = TAXI_IN;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);
				break; 
			}
		}
		case TAXI_IN:
		{
			{
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_ARRIVAL,lp);
				e = tw_event_new((lp -> tw_lp::gid),ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = ARRIVAL;
				m -> air_traffic_message::dest_region = (msg -> air_traffic_message::dest_region);
				m -> air_traffic_message::dest_airport = (msg -> air_traffic_message::dest_airport);
				m -> air_traffic_message::msg_from = (lp -> tw_lp::gid);
				tw_event_send(e);
				break; 
			}
		}
		case ARRIVAL:
		{
			{
				__store__< int  > ((s -> airport_state::runway_in_use),lp);
				s -> airport_state::runway_in_use--;
				int dest = (bs_rand_integer2(s -> airport_state::rn,NUMBER_OF_REGION_CONTROLLER,(NUMBER_OF_LP - 1),lp));
				ts = bs_rand_exponential2(s -> airport_state::rn,MEAN_LAND,lp);
				e = tw_event_new(dest,ts,lp);
				m = ((air_traffic_message *)(tw_event_data(e)));
				m -> air_traffic_message::type = DEP_REQ;
				tw_event_send(e);
				break; 
			}
		}
	}
}

void rc_event_handler(airport_state *s,tw_bf *bf,air_traffic_message *msg,tw_lp *lp)
{
	switch((msg -> air_traffic_message::type)){
		case DEP_REQ:
		{
			{
				//cout<<"RE-DEP_REQ"<<endl;
				int path = -1;
				__restore__< int  > (path,lp);
				(__builtin_expect((!(path >= 0)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",870,"path >= 0") : ((void )0);
				if (path) {
					bs_rand_rvs(s -> airport_state::rn,lp);
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::dep_req_accepted,lp);
					__restore__< int  > (s -> airport_state::runway_in_use,lp);
				}
				else {
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::dep_req_rejected,lp);
				}
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case DEP_DELAY:
		{
			{
				//cout<<"RE-DEP_DELAY"<<endl;
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case TAXI_OUT:
		{
			{
				//cout<<"RE-TAXI_OUT"<<endl;
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case TAKE_OFF:
		{
			{
				//cout<<"RE-TAKE_OFF"<<endl;
				bs_rand_rvs(s -> airport_state::rn,lp);
				__restore__< int  > (s -> airport_state::runway_in_use,lp);
				break; 
			}
		}
		case TRANSIT_REQ:
		{
			{
				//cout<<"RE-TRANSIT_REQ"<<endl;
				int path = -1;
				__restore__< int  > (path,lp);
				(__builtin_expect((!(path >= 0)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",926,"path >= 0") : ((void )0);
				if (path) {
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::transit_req_accepted,lp);
					__restore__< int  > (s -> airport_state::airplane_in_region,lp);
				}
				else {
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::transit_req_rejected,lp);
				}
				break; 
			}
		}
		case ON_THE_AIR:
		{
			{
				int path = -1;
				__restore__< int  > (path,lp);
				(__builtin_expect((!(path >= 0)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",948,"path >= 0") : ((void )0);
				if (path) {
					bs_rand_rvs(s -> airport_state::rn,lp);
				}
				else {
					bs_rand_rvs(s -> airport_state::rn,lp);
				}
				__restore__< int  > (s -> airport_state::airplane_in_region,lp);
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case TRANSIT_DELAY:
		{
			{
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case LANDING_REQ:
		{
			{
				int path = -1;
				__restore__< int  > (path,lp);
				(__builtin_expect((!(path >= 0)),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",978,"path >= 0") : ((void )0);
				if (path) {
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::landing_req_accepted,lp);
					__restore__< int  > (s -> airport_state::runway_in_use,lp);
				}
				else {
					bs_rand_rvs(s -> airport_state::rn,lp);
					__restore__< int  > (s -> airport_state::landing_req_rejected,lp);
				}
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case LANDING_DELAY:
		{
			{
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case LANDING:
		{
			{
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case TAXI_IN:
		{
			{
				bs_rand_rvs(s -> airport_state::rn,lp);
				break; 
			}
		}
		case ARRIVAL:
		{
			{
				bs_rand_rvs(s -> airport_state::rn,lp);
				bs_rand_rvs(s -> airport_state::rn,lp);
				__restore__< int  > (s -> airport_state::runway_in_use,lp);
				break; 
			}
		}
	}
}

void final(airport_state *s,tw_lp *lp)
{
	//wait_time_avg += ((s->waiting_time / (double) s->landings) / nlp_per_pe);
	total_transit_accepted += (s -> airport_state::transit_req_accepted);
	total_transit_rejected += (s -> airport_state::transit_req_rejected);
	total_dep_req_accepted += (s -> airport_state::dep_req_accepted);
	total_dep_req_rejected += (s -> airport_state::dep_req_rejected);
	total_landing_req_accepted += (s -> airport_state::landing_req_accepted);
	total_landing_req_rejected += (s -> airport_state::landing_req_rejected);
	//    cout<<lp->gid<<","<<s->arrival_req_accepted<<endl;
}
/*
 Parallel Running
 */
/*
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
 */
/*
 Sequential Running
 */
tw_lptype airport_lps[] = {{((init_f )p_init), ((event_f )event_handler_forward), ((revent_f )event_handler_reverse), ((final_f )final), ((map_f )mapping), ((sizeof(airport_state )))}, {(0)}};
static const tw_optdef app_opt[] = {{(TWOPTTYPE_GROUP), (0L), ("Airport Model"), (0L)}, 
	//TWOPT_UINT("nairports", nlp_per_pe, "initial # of airports(LPs)"),
	{(TWOPTTYPE_UINT), ("nplanes"), ("initial # of planes per airport(events)"), ((&planes_per_airport))}, {(TWOPTTYPE_STIME), ("mean"), ("mean flight time for planes"), ((&mean_flight_time))}, {(TWOPTTYPE_UINT), ("memory"), ("optimistic memory"), ((&opt_mem))}, {((tw_opttype )0), (0L), (0L), (0L)}};

int main(int argc,char **argv,char **env)
{
	cout << "##########"<<endl;
	cout << "Backstroke"<<endl;
	cout << "##########"<<endl;

	int i;
	tw_opt_add(app_opt);
	tw_init(&argc,&argv);
	nlp_per_pe /= ((tw_nnodes()) * g_tw_npe);
	(( *(&std::cout)<<"tw_nnode ") << tw_nnodes()) << std::endl< char  , std::char_traits< char  >  > ;
	(( *(&std::cout)<<"g_tw ") << g_tw_npe) << std::endl< char  , std::char_traits< char  >  > ;
	g_tw_events_per_pe = (((planes_per_airport * nlp_per_pe) / g_tw_npe) + opt_mem);
	tw_define_lps(nlp_per_pe,(sizeof(air_traffic_message )),0);
	/*
     init graph
     */
	( *(&std::cout)<<"buillding a graph") << std::endl< char  , std::char_traits< char  >  > ;
	graph = (::new Graph (20));
	graph ->  create_graph (GRAPH_CSV_FILE_PATH);
	//graph->print_adjmatrix();
	( *(&std::cout)<<"done") << std::endl< char  , std::char_traits< char  >  > ;
	/*
     We have two different LPs
     One represents an airport
     The other one represents Region Controller
     */
	for (i = 0; i < g_tw_nlp; i++) 
		tw_lp_settype(i,(airport_lps + 0));
	tw_run();
	if ((tw_ismaster())) {
		printf("\nAir Traffic Model Statistics:\n");
		printf("\t%-50s %11lld\n","Number of LPs (Airport, Region Controller)",((nlp_per_pe * g_tw_npe) * (tw_nnodes())));
		printf("\t%-50s %11lld\n","Number of planes",(((planes_per_airport * nlp_per_pe) * g_tw_npe) * (tw_nnodes())));
		(( *(&std::cout)<<"\tTotal Transit Accepted : ") << total_transit_accepted) << std::endl< char  , std::char_traits< char  >  > ;
		(( *(&std::cout)<<"\tTotal Transit Rejected : ") << total_transit_rejected) << std::endl< char  , std::char_traits< char  >  > ;
		(( *(&std::cout)<<"\tTotal Departure Req Accepted : ") << total_dep_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
		(( *(&std::cout)<<"\tTotal Departure Req Rejected : ") << total_dep_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
		(( *(&std::cout)<<"\tTotal Arrival Req Accepted : ") << total_landing_req_accepted) << std::endl< char  , std::char_traits< char  >  > ;
		(( *(&std::cout)<<"\tTotal Arrival Req Rejected : ") << total_landing_req_rejected) << std::endl< char  , std::char_traits< char  >  > ;
	}
	tw_end();
	if (1) {
		//	cout<<"Memory usage : "<<memusage<<" bytes,"<<" Store operations "<<store_operation<<" Restore operation "<<restore_operation<<endl;
		(std::cout << memusage<<",") << std::endl< char  , std::char_traits< char  >  > ;
	}
	return 0;
}

int get_region(int airport)
{
	if ((airport >= 20) && (airport <= 38)) {
		return 0;
	}
	else if ((airport >= 39) && (airport <= 53)) {
		return 1;
	}
	else if ((airport >= 54) && (airport <= 72)) {
		return 2;
	}
	else if ((airport >= 73) && (airport <= 92)) {
		return 3;
	}
	else if ((airport >= 93) && (airport <= 109)) {
		return 4;
	}
	else if ((airport >= 110) && (airport <= 114)) {
		return 5;
	}
	else if ((airport >= 115) && (airport <= 148)) {
		return 6;
	}
	else if ((airport >= 149) && (airport <= 166)) {
		return 7;
	}
	else if ((airport >= 167) && (airport <= 180)) {
		return 8;
	}
	else if ((airport >= 181) && (airport <= 201)) {
		return 9;
	}
	else if ((airport >= 202) && (airport <= 214)) {
		return 10;
	}
	else if ((airport >= 215) && (airport <= 226)) {
		return 11;
	}
	else if ((airport >= 227) && (airport <= 243)) {
		return 12;
	}
	else if ((airport >= 244) && (airport <= 254)) {
		return 13;
	}
	else if ((airport >= 0xff) && (airport <= 269)) {
		return 14;
	}
	else if ((airport >= 270) && (airport <= 294)) {
		return 15;
	}
	else if ((airport >= 295) && (airport <= 302)) {
		return 16;
	}
	else if ((airport >= 303) && (airport <= 318)) {
		return 17;
	}
	else if ((airport >= 319) && (airport <= 337)) {
		return 18;
	}
	else if ((airport >= 338) && (airport <= 347)) {
		return 19;
	}
	else {
		( *(&std::cout)<<"airport region failed") << std::endl< char  , std::char_traits< char  >  > ;
		(__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/cong/MyWork/newROSS/rossnet/trunk/ross/models/air_traffic/air_traffic.cpp",1247,"false") : ((void )0);
	}
}
