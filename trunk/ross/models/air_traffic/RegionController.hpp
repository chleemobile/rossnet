#include "Controller.hpp"
#include <algorithm>

class RegionController : public Controller
{
	public:
		RegionController(int in_max_capacity, int in_num_aircraft);
		virtual ~RegionController();

		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);
		void handle_aircraft(tw_lp *lp);

		void handle_incoming_forward(tw_lp *lp);
		void handle_incoming_reverse(tw_lp *lp);
		
		void handle_outgoing_forward(tw_lp *lp);
		void handle_outgoing_reverse(tw_lp *lp);
		
		void handle_aircraft_forward(tw_lp *lp);
		void handle_aircraft_reverse(tw_lp *lp);



		
};
