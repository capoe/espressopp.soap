#include "python.hpp"
#include "Portal.hpp"
#include "storage/DomainDecomposition.hpp"
#include "iterator/CellListIterator.hpp"

using namespace espressopp;
using namespace iterator;

namespace espressopp { namespace soap {
    
void Portal::registerPython() {
    using namespace espressopp::python;
    class_<Portal>("soap_Portal", init< shared_ptr< System > >())
        .def("initialise", &Portal::initialise);
}


real Portal::initialise() {
    System &system = getSystemRef();
    CellList cells = system.storage->getRealCells();
    for (CellListIterator cit(cells); !cit.isDone(); ++cit) {
        Particle &p = *cit;
        std::cout << p.mass() << std::endl;
    }
    return -1.;
}



}} /* Close namespaces */
