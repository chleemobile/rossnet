#include "RegionController.hpp"

RegionController::RegionController()
{
}

RegionController::~RegionController()
{
}

void RegionController::handle()
{
	cout<<"RC handle"<<endl;
	m_num_aircrafts++;
}
