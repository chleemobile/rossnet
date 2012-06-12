#include "Controller.hpp"

class RegionController : public Controller
{
	public:
		RegionController(int in_max_capacity);
		virtual ~RegionController();
		void handle_incoming();
		void handle_outgoing();

		//int m_max_capacity;
		//int m_num_aircrafts;
		
};
