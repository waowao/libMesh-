// $Id: error_estimator.h,v 1.4 2004-06-02 20:32:53 benkirk Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2004  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __error_estimator_h__
#define __error_estimator_h__

// C++ includes
#include <vector>
#include <string>

// Local Includes
#include "libmesh_common.h"
#include "equation_systems.h"
#include "steady_system.h"

/**
 * This class holds functions that will estimate the error
 * in a finite element solution on a given mesh.  These error
 * estimates can be useful in their own right, or may be used
 * to guide adaptive mesh refinement.  Note that in general
 * the computed errors are stored as floats rather than doubles
 * since the required precision is low.
 *
 * @author Benjamin S. Kirk, 2003.
 */
class ErrorEstimator
{
public:

  /**
   * Constructor. Empty.
   */
  ErrorEstimator() {}
  
  /**
   * Destructor.  
   */
  virtual ~ErrorEstimator() {}


  /**
   * This pure virtual function must be redefined
   * in derived classes to compute the error for each
   * cell and place it in the "error_per_cell" vector.
   */
  virtual void estimate_error (const SteadySystem& system,
			       std::vector<float>& error_per_cell) = 0;

  /**
   * When passed an EquationSystems reference, this function
   * first extracts the System named "name" and then calls the
   * function above.
   */
  virtual void estimate_error (const EquationSystems& es,
			       const std::string& name,
			       std::vector<float>& error_per_cell)
  { this->estimate_error (es.get_system<SteadySystem>(name), error_per_cell); }

  /**
   * This vector can be used to "mask" certain
   * variables in a system.  If the mask is empty then the error
   * will be calculated for all variables in the system.  If
   * the mask is not empty the error will be calculated only
   * for those components for which \p component_mask[c] is
   * true.
   */
  std::vector<bool> component_mask;

protected:

  /**
   * This method takes the local error contributions in
   * \p error_per_cell from each processor and combines
   * them to get the global error vector.
   */
  void reduce_error (std::vector<float>& error_per_cell) const;
};


#endif
