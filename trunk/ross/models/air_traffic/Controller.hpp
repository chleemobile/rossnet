#ifndef _controller_h_
#define _controller_h_

#include <iostream>
#include <queue>
#include <vector>
#include "Aircraft.hpp"

struct tw_lp;
//class Aircraft;

using namespace std;

class Controller
{
	public:
		Controller();
		Controller(int in_max_capacity, int in_num_aircraft);
		virtual ~Controller();

		//virtual void handle_incoming(tw_lp *lp);
		//virtual void handle_outgoing(tw_lp *lp);
		virtual void handle_aircraft(tw_lp *lp);
		virtual void handle_aircraft_forward(tw_lp *lp) = 0;
		virtual void handle_aircraft_reverse(tw_lp *lp) = 0;

	
	//protected:
		//int m_id;
		int m_max_capacity;
		int m_current_capacity;
		
		int m_aircraft_processed;
		int m_req_accepted;
		int m_req_rejected;

		int m_max_q_size;
		int m_max_counter;
		int m_mc_aircraft_id;

		double m_delay;
		int m_cdelay;

		priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > *m_in_q;
		vector<int> *m_counter;

};

#endif
