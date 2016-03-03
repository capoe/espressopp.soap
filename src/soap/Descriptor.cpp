#include <python.hpp>
#include "Descriptor.hpp"
#include "boost/foreach.hpp"

namespace espressopp {
  namespace soap {

    LOG4ESPP_LOGGER(Descriptor::logger, "Descriptor");

    python::list Descriptor::compute_real_vector_python() {
      python::list ret;
      compute_real_vector();
      BOOST_FOREACH(real value, result_real_vector) ret.append(value);
      return ret;
    }

    python::list Descriptor::compute_int_vector_python() {
      python::list ret;
      compute_int_vector();
      BOOST_FOREACH(int value, result_int_vector) ret.append(value);
      return ret;
    }

    //////////////////////////////////////////////////
    // REGISTRATION WITH PYTHON
    //////////////////////////////////////////////////
    void
    Descriptor::registerPython() {
      using namespace espressopp::python;
    
      class_< Descriptor, boost::noncopyable >("soap_Descriptor", no_init)
		.def("compute", &Descriptor::compute)
		.def("compute_real", &Descriptor::compute_real)
		.def("compute_int", &Descriptor::compute_int)
		.def("compute_real_vector_python", &Descriptor::compute_real_vector_python)
		.def("compute_int_vector_python",  &Descriptor::compute_int_vector_python)
		.def("getResultType", &Descriptor::getResultType)
      ;
    }
  }
}
