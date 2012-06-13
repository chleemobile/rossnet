#include "Controller.hpp"
#include "assert.h"
#include "rctypes.h" 

Controller::Controller()
{
}

Controller::Controller(int in_max_capacity)
{
  (this) -> m_max_capacity = in_max_capacity;
  (this) -> m_current_capacity = 0;
}

Controller::~Controller()
{
}

void Controller::handle_incoming()
{
  (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/Controller.cpp",21,"false") : ((void )0);
  ( *(&::std::cout)<<"CTR handle") << ::std::endl< char  , ::std::char_traits< char  >  > ;
}

void Controller::handle_incoming_forward()
{
  (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/Controller.cpp",21,"false") : ((void )0);
  ( *(&::std::cout)<<"CTR handle") << ::std::endl< char  , ::std::char_traits< char  >  > ;
}

void Controller::handle_incoming_reverse()
{
}

void Controller::handle_outgoing()
{
  (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/Controller.cpp",27,"false") : ((void )0);
  ( *(&::std::cout)<<"CTR handle") << ::std::endl< char  , ::std::char_traits< char  >  > ;
}

void Controller::handle_outgoing_forward()
{
  (__builtin_expect((!false),0))?__assert_rtn(__func__,"/Users/lee1017/dev/rossnet/trunk/ross/models/airport/Controller.cpp",27,"false") : ((void )0);
  ( *(&::std::cout)<<"CTR handle") << ::std::endl< char  , ::std::char_traits< char  >  > ;
}

void Controller::handle_outgoing_reverse()
{
}
