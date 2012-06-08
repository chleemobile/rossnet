#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft()
{
	m_id_generator++;
	m_id = m_id_generator;

	cout<<"Aircraft "<<m_id<<" had been creatd"<<endl;
	
}

Aircraft::~Aircraft()
{
}

int Aircraft::get_id()
{
	return m_id;
}
