#ifndef _SOAP_PORTAL_HPP
#define _SOAP_PORTAL_HPP
#include "python.hpp"
#include "types.hpp"
#include "SystemAccess.hpp"

namespace espressopp {
  namespace soap {
    class Portal : public SystemAccess
    {
    public:
       Portal(shared_ptr< System > system) : SystemAccess(system) {}
      ~Portal() {}
       real initialise();
       static void registerPython();
    };
  }
}

#endif
