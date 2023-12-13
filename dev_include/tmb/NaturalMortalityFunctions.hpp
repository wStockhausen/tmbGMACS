/** \file NaturalMortalityFunctions.hpp
 */
/**
 * File: NaturalMortalityFunctions.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * GMACS was originally developed using the AD Model Builder (ADMB) modeling
 * framework.
 *
 * This TMB version of GMACS is heavily indebted to the NOAA/NMFS Fisheries
 * Integrated Model System (FIMS) project for its framework and code.
 *
 * See LICENSE in the source folder for reuse information.
 *
*/

#ifndef GMACS_TMB_NATURALMORTALITYFUNCTIONS_HPP
#define GMACS_TMB_NATURALMORTALITYFUNCTIONS_HPP

#include "../tmbGMACS_types.hpp"

namespace gmacs{
/**
 * Returns a differentiable vector representing an M that does not vary across a range
 * of sizes
 *
 * @param zBs - DataVector with sizes at which to evaluate M
 * @param params - ParameterVector with parameter values
 *
 * M = p[0];
 */
template <class Type>
  VariableVector<Type> constantM(DataVector<real_t> zBs, ParameterVector<Type> p){
    VariableVector<Type> M = VariableVector<Type>(zBs.size());
    for (int i=0;i<zBs.size();i++) M[i] = p[0];
    return M;
  }//--constantM function
/**
 * Returns a differentiable vector representing natural mortality that varies
 * across a range of sizes based on Lorenzen's size-specific function.
 *
 * @param zBs - DataVector with sizes at which to evaluate M
 * @param params - ParameterVector with parameter values
 *    * p[0] - M at reference size p[1]
 *    * p[1] - reference size (fixed)
 *
 * M(z) = p[0]/(z/p[1]);
 *
 */
template <class Type>
  VariableVector<Type> lorenzenM(DataVector<real_t> zBs, ParameterVector<Type> p){
    VariableVector<Type> M = VariableVector<Type>(zBs.size());
    for (int i=0;i<zBs.size();i++) M[i] = p[0]/(zBs[i]/p[1]);
    return M;
  }//--lorenzenM function
}//--gmacs namespace
#endif //--GMACS_TMB_NATURALMORTALITYFUNCTIONS_HPP
