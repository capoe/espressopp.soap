#include "python.hpp"
#include <cmath>
#include "Soap.hpp"
#include "storage/DomainDecomposition.hpp"
#include "iterator/CellListIterator.hpp"

using namespace espressopp;
using namespace iterator;

namespace espressopp {
  namespace soap {
    
    void Soap::registerPython() {
      using namespace espressopp::python;
      class_<Soap, bases< Descriptor > >
        ("soap_Soap", init< shared_ptr< System > >())
        .def("compute", &Soap::compute)
      ;
    }
  }
}
