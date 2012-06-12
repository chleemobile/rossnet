#include "Controller.hpp"

class LocalTrafficController : public Controller
{
	public:
		LocalTrafficController();
		virtual ~LocalTrafficController();
		void handle();

		int m_max_runway;
		int m_num_runways_used;

};

