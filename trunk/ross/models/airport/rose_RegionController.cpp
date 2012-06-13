#include "RegionController.hpp"
//#include "rctypes.h" 

RegionController::RegionController(int in_max_capacity)
{
  (this) -> m_max_capacity = in_max_capacity;
  (this) -> m_current_capacity = 0;
}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming()
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_forward()
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_reverse()
{
  --m_current_capacity;
  --m_current_capacity;
}

void RegionController::handle_outgoing()
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_forward()
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_reverse()
{
  ++m_current_capacity;
  ++m_current_capacity;
}
