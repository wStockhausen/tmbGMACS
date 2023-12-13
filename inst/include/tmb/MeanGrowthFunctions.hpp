/** \file MeanGrowthFunctions.hpp
 */
/**
 * File: MeanGrowthFunctions.hpp
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

#ifndef GMACS_TMB_MEANGROWTHFUNCTIONS_HPP
#define GMACS_TMB_MEANGROWTHFUNCTIONS_HPP

#include "../tmbGMACS_traits.hpp"

namespace gmacs{
/**
 * Returns a differentiable vector representing the mean growth from
 * pre-molt size based on parameters
 *  * p[0] - grA: ln-scale intercept
 *  * p[1] - grB: ln-scale rate-of-increase
 * Mean growth could be interpreted as either molt increment or post-molt size,
 * depending on context.
 *
 * @param preZBs - DataVector with pre-molt sizes at which to evaluate mean growth
 * @param p - ParameterVector with parameter values
 * @param consts - constants (ignored here)
 *
 * mnZs(z) = exp(grA+grB*log(z))
 *
 */
template <class Type>
  VariableVector<Type> mnGrw1(DataVector<real_t> preZBs,
                              ParameterVector<Type> p,
                              DataVector<real_t> consts){
    Rcout<<"in mnGrw1"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
    VariableVector<Type> mnZs = ParameterVector<Type>(preZBs.size());
    for (int i = 0;i<preZBs.size();i++)
      mnZs[i] = grA*exp(grB*log(preZBs[i]));
    return mnZs;
  }//--mnGrw1
/**
 * Returns a differentiable vector representing the mean growth from
 * pre-molt size based on parameters
 *  * p[0] - grA: mean growth at zGrA
 *  * p[1] - grB: mean growth at zGrB
 * Mean growth can be interpreted as either molt increment or post-molt size,
 * depending on context.
 *
 * @param preZBs - DataVector with pre-molt sizes at which to evaluate mean growth
 * @param p - ParameterVector with parameter values
 * @param consts - constants
 *  * consts[0] - zGrA: pre-molt size corresponding to grA
 *  * consts[1] - zGrB: pre-molt size corresponding to grB
 *
 * mnZ(z) = grA*exp(log(grB/grA)/log(zGrB/zGrA)*log(z/zGrA));
 *
 */
template <class Type>
  VariableVector<Type> mnGrw2(DataVector<real_t> preZBs,
                              ParameterVector<Type> p,
                              DataVector<real_t> consts){
    Rcout<<"in mnGrw2"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
    VariableVector<Type> mnZs = ParameterVector<Type>(preZBs.size());
    for (int i = 0;i<preZBs.size();i++)
      mnZs[i] = grA*exp(log(grB/grA)/log(zGrB/zGrA)*log(preZBs[i]/zGrA));
    return mnZs;
  }//--mnGrw2
/**
 * Returns a differentiable vector representing the mean growth from
 * pre-molt size based on parameters
 *  * p[0] - grA: mean growth at pre-molt size zGrA
 *  * p[1] - grB: ln-scale slope of pre-/post-molt size relationship
 * Mean growth can be interpreted as either molt increment or post-molt size,
 * depending on context.
 *
 * @param preZBs - DataVector with pre-molt sizes at which to evaluate mean growth
 * @param p - ParameterVector with parameter values
 * @param consts - DataVector with function constants
 *  * consts[0] - zGrA: pre-molt size corresponding to grA
 *
 * mnZ(z) = grA*exp(grB*log(z/zGrA));
 *
 */
template <class Type>
  VariableVector<Type> mnGrw3(DataVector<real_t> preZBs,
                              ParameterVector<Type> p,
                              DataVector<real_t> consts){
    Rcout<<"in mnGrw3"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
    VariableVector<Type> mnZs = ParameterVector<Type>(preZBs.size());
    for (int i = 0;i<preZBs.size();i++)
      mnZs[i] = grA*exp(grB*log(preZBs[i]/zGrA));
    return mnZs;
  }//--mnGrw3
}//--gmacs namespace
#endif //--GMACS_TMB_MEANGROWTHFUNCTIONS_HPP
