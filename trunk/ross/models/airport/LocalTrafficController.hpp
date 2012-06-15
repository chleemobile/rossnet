#include "Controller.hpp"


class LocalTrafficController : public Controller
{
	public:
		LocalTrafficController(int in_max_capacity);
		virtual ~LocalTrafficController();
		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);

		void handle_incoming_forward(tw_lp *lp);
		void handle_incoming_reverse(tw_lp *lp);
		void handle_outgoing_forward(tw_lp *lp);
		void handle_outgoing_reverse(tw_lp *lp);

		//int m_max_runway;
		//int m_num_runways_used;

};

