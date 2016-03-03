#ifndef _SOAP_SOAP_HPP
#define _SOAP_SOAP_HPP
#include "Descriptor.hpp"

namespace espressopp {
  namespace soap {
    class Soap : public Descriptor
    {
    public:
       Soap(shared_ptr< System > system) : Descriptor(system) {}
      ~Soap() {}
       virtual real compute() const { return -1.; }
       static void registerPython();
    };
  }
}

#endif
