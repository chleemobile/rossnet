#include "Aircraft.hpp"

int Aircraft::m_id_generator = 0;

Aircraft::Aircraft()
{
    m_id_generator++;
    m_id = m_id_generator++;
}

Aircraft::~Aircraft()
{
    
}


void Aircraft::set_dest_airport(int dest_airport)
{
    m_dest_airport = dest_airport;
}

void Aircraft::set_dest_region(int dest_region)
{
    m_dest_region = dest_region;
}

int Aircraft::get_dest_airport()
{
    return m_dest_airport;
}

int Aircraft::get_dest_region()
{
    return m_dest_region;
}

int Aircraft::get_id()
{
    return m_id;
}

int Aircraft::get_next_region()
{
    return m_dest_region;
}