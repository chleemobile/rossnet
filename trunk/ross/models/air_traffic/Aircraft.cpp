#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft():m_src_airport(0), m_src_region(0),
	m_process_time(0), m_dest_region(0), m_dest_airport(0),
	m_max_speed(0), m_speed(0), m_remaining_dist(0),
	//m_path(10),
	m_clock(0), m_delay(0), m_cdelay(0)
{
	m_id_generator++;
	m_id = m_id_generator;

	//cout<<"Aircraft "<<m_id<<" had been creatd"<<endl;

}

Aircraft::~Aircraft()
{

}

/*
Aircraft& Aircraft::operator=( Aircraft &src)
{
	//cout<<"copy contructor overloading"<<endl;
	m_id = src.m_id;
	m_process_time = src.m_process_time;
	m_src_airport = src.m_src_airport;
	m_src_region = src.m_src_region;
	m_dest_region = src.m_dest_region;
	m_dest_airport = src.m_dest_airport ;

	m_max_speed = src.m_max_speed;
	m_speed = src.m_speed;

	if(src.m_remaining_dist <= 0)
		m_remaining_dist = 0;
	else m_remaining_dist = src.m_remaining_dist;

	m_clock = src.m_clock;	

	m_delay = src.m_delay;
	m_cdelay = src.m_cdelay;

	return *this;
}
*/

bool Aircraft::operator<(const Aircraft &rhs) const
{
#if 0
	return m_process_time > rhs.m_process_time;
#else
	if (m_process_time == rhs.m_process_time)
        return m_id < rhs.m_id;
    else
        return m_process_time > rhs.m_process_time;
#endif
}

/*
   void Aircraft::print_path() const
   {
   int size = m_path.size();

   cout<<m_src_region+1<<"->"<<m_dest_region+1<<";";
   for(int i=0; i < size; i++)
   {
   cout<<m_path[i]+1<<",";
   }
   cout<<""<<endl;
   }
 */
