#ifndef _SOAP_PORTAL_HPP
#define _SOAP_PORTAL_HPP
#include "python.hpp"
#include "types.hpp"
#include "SystemAccess.hpp"
#include "Real3D.hpp"
#include <boost/format.hpp>

namespace espressopp {
    namespace soap {

class Options
{
public:
	Options()
		: _N_real(8), _L_real(6), _Rc_real(5.),
		  _N_recip(8), _L_recip(6), _Rc_recip(5.),
		  _b1(Real3D(1,0,0)), _b2(Real3D(0,1,0)), _b3(Real3D(0,0,1)),
		  _center_w0(1.), _center_excludes(boost::python::list()) { ; }
   ~Options() {}

	void configureRealBasis(int N_real, int L_real, real Rc_real) {
		_N_real = N_real;
		_L_real = L_real;
		_Rc_real = Rc_real;
	}
	void configureReciprocalBasis(int N_recip, int L_recip, real Rc_recip) {
		_N_recip = N_recip;
		_L_recip = L_recip;
		_Rc_recip = Rc_recip;
	}
	void configureCenters(real center_w0, boost::python::list center_excludes) {
		_center_w0 = center_w0;
		_center_excludes = center_excludes;
	}
	void configureReciprocalLattice(Real3D b1, Real3D b2, Real3D b3) {
		_b1 = b1;
		_b2 = b2;
		_b3 = b3;
	}
	std::string summarizeOptions() {
		std::string info = "";
		info += "Options:\n";
		info += "o Centers: \n";
		info += (boost::format("    W0=%1% Excl#=%2%\n") % _center_w0 % boost::python::len(_center_excludes)).str();
		info += "o Real-space basis\n";
		info += (boost::format("    N=%1% L=%2% Rc=%3%\n") % _N_real % _L_real % _Rc_real).str();
		info += "o Reciprocal-space basis\n";
		info += (boost::format("    N=%1% L=%2% Rc=%3%\n") % _N_recip % _L_recip % _Rc_recip).str();
		info += "o Reciprocal-space lattice\n";
		info += (boost::format("    b1x=%1% b1y=%2% b1z=%3%\n") % _b1[0] % _b1[1] % _b1[2]).str();
		info += (boost::format("    b2x=%1% b2y=%2% b2z=%3%\n") % _b2[0] % _b2[1] % _b2[2]).str();
		info += (boost::format("    b3x=%1% b3y=%2% b3z=%3%") % _b3[0] % _b3[1] % _b3[2]).str();

		return info;
	}
	static void registerPython();

private:
	// REAL SPACE
	int _N_real;
	int _L_real;
	real _Rc_real;
	// RECIPROCAL SPACE
	int _N_recip;
	int _L_recip;
	real _Rc_recip;
	Real3D _b1;
	Real3D _b2;
	Real3D _b3;
	// CENTERS
	real _center_w0;
	boost::python::list _center_excludes;

};

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
