#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft()
{
	m_id_generator++;

	m_id = m_id_generator;
	m_dest = 0;
	m_dep_time = 0;

	m_sdelay = 0;
	m_wclock = 0;
	m_wdelay = 0;

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

void Aircraft::set_dest(const int dest)
{
	m_dest = dest;
}

void Aircraft::set_wclock(const double now)
{
	m_wclock = now;
}


void Aircraft::increase_sdelay()
{
	m_sdelay++;
	assert(m_sdelay > 0);
}

void Aircraft::calculate_wdelay(const double now)
{
	m_wdelay = m_wdelay + ( now - m_wclock);
	m_wclock = now;
	assert(m_wdelay >= 0);
}

double Aircraft::get_wdelay()
{
	return m_wdelay;
}

int Aircraft::get_sdelay()
{
	return m_sdelay;
}

int Aircraft::get_id()
{
	return m_id;
}

int Aircraft::get_dest()
{
	return m_dest;
}

void Aircraft::reset_delays()
{
	m_sdelay = 0;
	m_wdelay = 0;
	m_wclock = 0;
}
