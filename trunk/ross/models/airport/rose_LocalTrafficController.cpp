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

void LocalTrafficController::handle_incoming(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
}

void LocalTrafficController::handle_incoming_forward(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity++;
}

void LocalTrafficController::handle_incoming_reverse(struct tw_lp *lp)
{
  --m_current_capacity;
}

void LocalTrafficController::handle_outgoing(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_forward(struct tw_lp *lp)
{
//cout<<"LTC handle"<<endl;
  (this) -> m_current_capacity--;
}

void LocalTrafficController::handle_outgoing_reverse(struct tw_lp *lp)
{
  ++m_current_capacity;
}
