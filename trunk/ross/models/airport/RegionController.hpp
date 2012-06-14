#include "Controller.hpp"

class RegionController : public Controller
{
	public:
		RegionController(int in_max_capacity);
		virtual ~RegionController();
		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);
	
		void handle_incoming_forward();
		void handle_incoming_reverse();
		void handle_outgoing_forward();
		void handle_outgoing_reverse();

		//int m_max_capacity;
		//int m_num_aircrafts;
		
};
