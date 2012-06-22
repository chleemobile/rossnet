#include "RegionController.hpp"
#include <ross.h>
#undef min
#undef max
#include "rctypes.h" 

RegionController::RegionController(int in_max_capacity)
{
  (this) -> m_max_capacity = in_max_capacity;
  (this) -> m_current_capacity = 0;
  (this) -> dummy_test = 1.2345;
}

RegionController::~RegionController()
{
}

void RegionController::handle_incoming(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_forward(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity++;
//dm_num_aircrafts++;
}

void RegionController::handle_incoming_reverse(struct tw_lp *lp)
{
  --m_current_capacity;
}

void RegionController::handle_outgoing(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_forward(struct tw_lp *lp)
{
//cout<<"RC handle"<<endl;
  (this) -> m_current_capacity--;
//dm_num_aircrafts++;
}

void RegionController::handle_outgoing_reverse(struct tw_lp *lp)
{
  ++m_current_capacity;
}
