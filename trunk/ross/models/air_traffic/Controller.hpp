#ifndef _controller_h_
#define _controller_h_

#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Aircraft.hpp"

struct tw_lp;
using namespace std;

#define BS

class Controller
{
	public:
		explicit Controller();
		explicit Controller(int in_max_capacity, int in_num_aircraft);
		virtual ~Controller();

		virtual void handle_incoming(tw_lp *lp) = 0;
		virtual void handle_outgoing(tw_lp *lp)= 0;
		virtual void handle_aircraft(tw_lp *lp) = 0;

		void add_aircraft(Aircraft aircraft, tw_lp *lp);
		Aircraft get_aircraft(tw_lp *lp) const;
		void remove_aircraft(tw_lp *lp);

#ifdef BS		
		virtual void handle_incoming_forward(tw_lp *lp)=0;
		virtual void handle_incoming_reverse(tw_lp *lp)=0;

		virtual void handle_outgoing_forward(tw_lp *lp)=0;
		virtual void handle_outgoing_reverse(tw_lp *lp)=0;

		virtual void handle_aircraft_forward(tw_lp *lp)=0;
		virtual void handle_aircraft_reverse(tw_lp *lp)=0;

		Aircraft get_aircraft_forward(tw_lp *lp) const;
		Aircraft get_aircraft_reverse(tw_lp *lp) const;

		void add_aircraft_forward(Aircraft aircraft, tw_lp *lp);
		void add_aircraft_reverse(tw_lp *lp);

		void remove_aircraft_forward(tw_lp *lp);
		void remove_aircraft_reverse(tw_lp *lp);
#endif
		
		//protected:
		
		int m_id;
		int m_max_capacity;
		int m_current_capacity;
		int m_aircraft_processed;

		double dummy_test;
		
//		priority_queue < Aircraft, vector<Aircraft>, less<Aircraft> > m_q;
//		vector <Aircraft> m_q;
		deque <Aircraft> m_q;
		//vector<int> m_counter;
};


#endif
