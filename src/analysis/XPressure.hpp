/*
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
#ifndef _ANALYSIS_XPRESSURE_HPP
#define _ANALYSIS_XPRESSURE_HPP

#include "types.hpp"
#include "Observable.hpp"

#include "python.hpp"

namespace espressopp {
  namespace analysis {
    // Class to compute the pressure profile along slabs in the x-direction of the system.
    class XPressure : public Observable {
    public:
      XPressure(shared_ptr< System > system) : Observable(system) {}
      ~XPressure() {}
      virtual real compute() const;
      virtual python::list computeArray(int) const;

      static void registerPython();
    };
  }
}

#endif
