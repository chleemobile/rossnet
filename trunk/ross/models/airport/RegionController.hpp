#include "Controller.hpp"

class RegionController : protected Controller
{
	public:
		RegionController();
		virtual ~RegionController();
		void handle();
};
