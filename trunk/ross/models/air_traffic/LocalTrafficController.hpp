#include "Controller.hpp"

class LocalTrafficController : public Controller
{
	public:
		LocalTrafficController(int in_max_capacity, int in_num_aircraft);
		virtual ~LocalTrafficController();

		void handle_incoming(tw_lp *lp);
		void handle_outgoing(tw_lp *lp);
		void handle_aircraft(tw_lp *lp);
		Aircraft get_aircraft(tw_lp *lp);
		void remove_aircraft(tw_lp *lp);
		void add_aircraft(Aircraft aircraft, tw_lp *lp);

		void handle_incoming_forward(tw_lp *lp);
		void handle_incoming_reverse(tw_lp *lp);
		
		void handle_outgoing_forward(tw_lp *lp);
		void handle_outgoing_reverse(tw_lp *lp);
		
		void handle_aircraft_forward(tw_lp *lp);
		void handle_aircraft_reverse(tw_lp *lp);

		Aircraft get_aircraft_forward(tw_lp *lp);
		Aircraft get_aircraft_reverse(tw_lp *lp);

		void add_aircraft_forward(Aircraft aircraft, tw_lp *lp);
		void add_aircraft_reverse(tw_lp *lp);

		void remove_aircraft_forward(tw_lp *lp);
		void remove_aircraft_reverse(tw_lp *lp);

	private:
		template <class T>	
		bool sort_predicate(const T &lhs, const T &rhs);
	
};

