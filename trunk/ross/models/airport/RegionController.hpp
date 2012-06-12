#include "Controller.hpp"

class RegionController : public Controller
{
	public:
		RegionController();
		virtual ~RegionController();
		void handle();

		int m_max_capacity;
		int m_num_aircrafts;
		
};
