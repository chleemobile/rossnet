#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft()
{
	m_id_generator++;

	m_id = m_id_generator;
	m_dest = 0;
	m_delay_time = 0;

	//cout<<"Aircraft "<<m_id<<" had been creatd"<<endl;
	
}

Aircraft::~Aircraft()
{
}

Aircraft& Aircraft::operator=( Aircraft &src)
{
	//cout<<"copy contructor overloading"<<endl;
	m_id = src.m_id;
	return *this;
}


void Aircraft::set_dest(const int dest)
{
	m_dest = dest;
}

int Aircraft::get_id()
{
	return m_id;
}

int Aircraft::get_dest()
{
	return m_dest;
}
