#include "Controller.hpp"

class RegionController : public Controller
{
	public:
		RegionController(int in_max_capacity);
		virtual ~RegionController();
		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);
	
		void handle_incoming_forward(tw_lp *lp);
		void handle_incoming_reverse(tw_lp *lp);
		void handle_outgoing_forward(tw_lp *lp);
		void handle_outgoing_reverse(tw_lp *lp);

		//int m_max_capacity;
		//int m_num_aircrafts;
		
};
