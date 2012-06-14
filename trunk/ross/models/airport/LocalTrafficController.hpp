#include "Controller.hpp"


class LocalTrafficController : public Controller
{
	public:
		LocalTrafficController(int in_max_capacity);
		virtual ~LocalTrafficController();
		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);

		void handle_incoming_forward();
		void handle_incoming_reverse();
		void handle_outgoing_forward();
		void handle_outgoing_reverse();

		//int m_max_runway;
		//int m_num_runways_used;

};

