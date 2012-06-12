#include "RegionController.hpp"

RegionController::RegionController(int in_max_capacity)
{
	m_max_capacity = in_max_capacity;
	m_current_capacity = 0;

}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming()
{
	//cout<<"RC handle"<<endl;
	m_current_capacity++;
	m_current_capacity++;

	//dm_num_aircrafts++;
}

void RegionController::handle_outgoing()
{
	//cout<<"RC handle"<<endl;
	m_current_capacity--;
	m_current_capacity--;

	//dm_num_aircrafts++;
}

