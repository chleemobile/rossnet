#ifndef _controller_h_
#define _controller_h_

#include <iostream>

using namespace std;

class Controller
{
	public:
		Controller();
		virtual ~Controller();
		virtual void handle();
	
	protected:
		int m_id;
		int get_id();

};

#endif
