#ifndef _controller_h_
#define _controller_h_

#include <iostream>

using namespace std;

class Controller
{
	public:
		Controller();
		Controller(int in_max_capacity);
		virtual ~Controller();
		virtual void handle_incoming();
		virtual void handle_outgoing();

		//virtual void handle_incoming_forward()=0;
		//virtual void handle_incoming_reverse()=0;
		//virtual void handle_outgoing_forward()=0;
		//virtual void handle_outgoing_reverse()=0;
	
	//protected:
		int m_id;
		int m_max_capacity;
		int m_current_capacity;

		int get_id();

};

#endif
