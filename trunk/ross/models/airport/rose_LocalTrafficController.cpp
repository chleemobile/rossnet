#include "LocalTrafficController.hpp"
//#include "rctypes.h" 

LocalTrafficController::LocalTrafficController(int in_max_capacity)
{
  (this) -> m_max_capacity = in_max_capacity;
  (this) -> m_current_capacity = 0;
}

LocalTrafficController::~LocalTrafficController()
{
}

void LocalTrafficController::handle_incoming()
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
}

void LocalTrafficController::handle_incoming_forward()
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
}

void LocalTrafficController::handle_incoming_reverse()
{
  --m_current_capacity;
}

void LocalTrafficController::handle_outgoing()
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_forward()
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_reverse()
{
  ++m_current_capacity;
}
