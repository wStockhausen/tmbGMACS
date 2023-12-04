/** \file GrowthFunctions.hpp
 */
/**
 * File: GrowthFunctions.hpp
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

#ifndef GMACS_TMB_GROWTHFUNCTIONS_HPP
#define GMACS_TMB_GROWTHFUNCTIONS_HPP

// #include <tmb_enable_header_only.hpp>
// #include <TMB.hpp>
#include "../tmbGMACS_types.hpp"
#include "../gmacs_utils.hpp"
// #include <Rstream.hpp>
// #include <cppad/cppad.hpp>
// #include <Vectorize.hpp>
// #include <distributions_R.hpp>

// // Re-define TMB_OBJECTIVE_PTR
// #undef  TMB_OBJECTIVE_PTR
// #define TMB_OBJECTIVE_PTR obj
// template<class Type>
// Type foo (objective_function<Type> *obj) {
//   return Type(0);
// }
// // Restore default TMB_OBJECTIVE_PTR
// #undef  TMB_OBJECTIVE_PTR
// #define TMB_OBJECTIVE_PTR this

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
    Rcpp::Rcout<<"in mnGrw1"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcpp::Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
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
    Rcpp::Rcout<<"in mnGrw2"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcpp::Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
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
    Rcpp::Rcout<<"in mnGrw3"<<std::endl;
    Type grA = p[0];
    Type grB = p[1];
    real_t zGrA = consts[0];
    real_t zGrB = consts[1];
    Rcpp::Rcout<<grA<<" "<<grB<<" "<<zGrA<<" "<<zGrB<<" "<<std::endl;
    VariableVector<Type> mnZs = ParameterVector<Type>(preZBs.size());
    for (int i = 0;i<preZBs.size();i++)
      mnZs[i] = grA*exp(grB*log(preZBs[i]/zGrA));
    return mnZs;
  }//--mnGrw3
// /**
//  * Returns a differentiable vector representing the probability of growth from
//  * pre-molt to  post-molt size, given that molting occurred.
//  *  * p[0] - mean growth parameter grA
//  *  * p[1] - mean growth parameter grB
//  *  * p[2] - gamma distribution scale factor
//  *
//  * @param preZBs - DataVector with pre-molt size bin midpoints
//  * @param pstZCs - DataVector with post-molt size bin cutpoints
//  * @param p - ParameterVector with parameter values
//  * @param consts - constants (ignored here)
//  */
// template <class Type>
//   VariableMatrix<Type> probGrw1(DataVector<real_t> preZBs,
//                                 DataVector<real_t> pstZCs,
//                                 ParameterVector<Type> p,
//                                 DataVector<real_t> consts){
//     Rcpp::Rcout<<"in probGrw1"<<std::endl;
//     Type grBeta = p[2];
//     VariableVector<Type> mnZs = gmacs::mnGrw1(preZBs,p,consts);
//     Rcpp::Rcout<<"mnZs = "<<mnZs<<std::endl;
//     VariableMatrix<Type> prGrw = VariableMatrix<Type>(preZBs.size(),pstZCs.size()-1);
//     for (int i=0;i<preZBs.size();i++) {
//       Type alpha = (mnZs[i]-preZBs[i])/grBeta;//--gamma distribution shape parameter
//       Type p0 = Type(0);
//       for (int j=0;j<pstZCs.size();j++) {
//         prGrw(i,j) = Type(0);
//         if (preZBs[i]<pstZCs[j]) {
//           //pgamma(q,shape,scale) (see include/distribution_R.hpp)
//           Type q  = (pstZCs[j]-preZBs[i])/grBeta;
//           Type p1 = pgamma<Type>(q,alpha);
//           prGrw(i,j) = p1-p0;//scale = 1
//           Rcpp::Rcout<<i<<" "<<j<<" "<<preZBs[i]<<" "<<pstZCs[j]<<" "<<alpha<<" "<<q<<" "<<p1<<" "<<p0<<" "<<prGrw(i,j)<<std::endl;
//           p0 = p1;
//         }
//       }//--j
//       //last post-molt size bin acts as accumulator
//       //--p0 is cumulative prob up to pstZCs[last]
//       int j = pstZCs.size()-1;
//       prGrw(i,j) += Type(1) - p0;
//       Rcpp::Rcout<<i<<" "<<j<<" "<<preZBs[i]<<" "<<pstZCs[j]<<" "<<alpha<<" "<<Type(1)-p0<<" "<<prGrw(i,j)<<std::endl;
//     }//--i
//     return prGrw;
//   }//--probGrw1 function
}//--gmacs namespace
#endif //--GMACS_TMB_GROWTHFUNCTIONS_HPP
