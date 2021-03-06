/*
  Copyright (C) 2014 Pierre de Buyl
  Copyright (C) 2012,2013
      Max Planck Institute for Polymer Research
  Copyright (C) 2008,2009,2010,2011
      Max-Planck-Institute for Polymer Research & Fraunhofer SCAI
  
  This file is part of ESPResSo++.
  
  ESPResSo++ is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ESPResSo++ is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

// ESPP_CLASS
#ifndef _ANALYSIS_TOTAL_VELOCITY_HPP
#define _ANALYSIS_TOTAL_VELOCITY_HPP

#include "types.hpp"
#include "Real3D.hpp"
#include "ParticleAccess.hpp"

namespace espressopp {
  namespace analysis {

    /** Class to compute the total velocity of a system. TotalVelocity provides
	a facility to reset the total velocity of the system.
    */

    class TotalVelocity : public ParticleAccess {

    public:

      TotalVelocity(shared_ptr<System> system) : ParticleAccess (system) {}

      ~TotalVelocity() {}

      /** Compute the total velocity of the system*/
      void compute();

      /** Reset the total velocity of the system*/
      void reset();

      void perform_action() { reset(); }

      Real3D getV() const { return v; }

      static void registerPython();

    protected:

      static LOG4ESPP_DECL_LOGGER(logger);

    private:

      Real3D v;
    };
  }
}

#endif
