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

#ifndef GMACS_TMB_PROBGROWTHFUNCTIONS_HPP
#define GMACS_TMB_PROBGROWTHFUNCTIONS_HPP

// #include <tmb_enable_header_only.hpp>
// #include <TMB.hpp>
#include "../tmbGMACS_types.hpp"
#include "../gmacs_utils.hpp"
#include "MeanGrowthFunctions.hpp"
// #include <Rstream.hpp>
// #include <cppad/cppad.hpp>
// #include <Vectorize.hpp>
// #include <distributions_R.hpp>

namespace gmacs{
/**
 * Returns a differentiable vector representing the probability of growth from
 * pre-molt to  post-molt size, given that molting occurred.
 *  * p[0] - mean growth parameter grA
 *  * p[1] - mean growth parameter grB
 *  * p[2] - gamma distribution scale factor
 *
 * @param preZBs - DataVector with pre-molt size bin midpoints
 * @param pstZCs - DataVector with post-molt size bin cutpoints
 * @param p - ParameterVector with parameter values
 * @param consts - constants (ignored here)
 */
template <class Type>
  VariableMatrix<Type> probGrw1(DataVector<real_t> preZBs,
                                DataVector<real_t> pstZCs,
                                ParameterVector<Type> p,
                                DataVector<real_t> consts){
    Rcpp::Rcout<<"in probGrw1"<<std::endl;
    Type grBeta = p[2];
    VariableVector<Type> mnZs = gmacs::mnGrw1(preZBs,p,consts);
    Rcpp::Rcout<<"mnZs = "<<mnZs<<std::endl;
    VariableMatrix<Type> prGrw = VariableMatrix<Type>(preZBs.size(),pstZCs.size()-1);
    for (int i=0;i<preZBs.size();i++) {
      Type alpha = (mnZs[i]-preZBs[i])/grBeta;//--gamma distribution shape parameter
      Type p0 = Type(0);
      for (int j=0;j<pstZCs.size();j++) {
        prGrw(i,j) = Type(0);
        if (preZBs[i]<pstZCs[j]) {
          //pgamma(q,shape,scale) (see TMB include/distribution_R.hpp)
          Type q  = (pstZCs[j]-preZBs[i])/grBeta;
          Type p1 = pgamma<Type>(q,alpha);
          prGrw(i,j) = p1-p0;//scale = 1
          Rcpp::Rcout<<i<<" "<<j<<" "<<preZBs[i]<<" "<<pstZCs[j]<<" "<<alpha<<" "<<q<<" "<<p1<<" "<<p0<<" "<<prGrw(i,j)<<std::endl;
          p0 = p1;
        }
      }//--j
      //last post-molt size bin acts as accumulator
      //--p0 is cumulative prob up to pstZCs[last]
      int j = pstZCs.size()-1;
      prGrw(i,j) += Type(1) - p0;
      Rcpp::Rcout<<i<<" "<<j<<" "<<preZBs[i]<<" "<<pstZCs[j]<<" "<<alpha<<" "<<Type(1)-p0<<" "<<prGrw(i,j)<<std::endl;
    }//--i
    return prGrw;
  }//--probGrw1 function
}//--gmacs namespace

RCPP_MODULE(probgrwfcns_module) {
  Rcpp::function("probGrw1",  &gmacs::probGrw1<Type>, "growth probability function 1");
}//--probgrwfcns_module

#endif //--GMACS_TMB_PROBGROWTHFUNCTIONS_HPP
