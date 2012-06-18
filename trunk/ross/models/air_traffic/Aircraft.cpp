#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft():m_src_airport(0), m_src_region(0),
					 m_dep_time(0), m_dest_region(0), m_dest_airport(0),
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

//Aircraft& Aircraft::operator=( Aircraft &src)
//{
	//cout<<"copy contructor overloading"<<endl;
//	m_id = src.m_id;
//	return *this;
//}

bool Aircraft::operator<(const Aircraft &rhs) const
{
	return (m_dep_time > rhs.m_dep_time);
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
