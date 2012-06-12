#include "Controller.hpp"

class LocalTrafficController : public Controller
{
	public:
		LocalTrafficController(int in_max_capacity);
		virtual ~LocalTrafficController();
		void handle_incoming();
		void handle_outgoing();

		//int m_max_runway;
		//int m_num_runways_used;

};

