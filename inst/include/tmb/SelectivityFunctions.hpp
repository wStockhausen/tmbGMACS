/** \file SelectivityFunctions.hpp
 */
/**
 * File: SelectivityFunctions.hpp
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

#ifndef GMACS_TMB_SELECTIVITYFUNCTIONS_HPP
#define GMACS_TMB_SELECTIVITYFUNCTIONS_HPP

#include "../tmbGMACS_types.hpp"
#include "../gmacs_utils.hpp"
#include <tmbutils/splines.hpp>

namespace gmacs{
/**
 * Returns a differentiable vector representing selectivity that does not vary across a range
 * of sizes, with parameter
 *  * p[0] - value
 *
 * @param zBs - DataVector with sizes at which to evaluate the selectivity
 * @param p - ParameterVector with parameter values
 * @param consts - constants (ignored here)
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *
 * S(z) = 0      : if z < z_min or z_max < z
 * S(z) = p[0]   : z otherwise
 */
template <class Type>
  VariableVector<Type> constantSel(DataVector<real_t> zBs,
                                   ParameterVector<Type> p,
                                   DataVector<real_t> consts){
    VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    for (int i=0;i<zBs.size();i++) {
      S[i] = w[i]*p[0];
    }
    return S;
  }//--constantSel function
/**
 * ascLogisticSel1: Returns a differentiable vector representing selectivity that varies
 * across a range of sizes based on an ascending logistic curve with parameters
 *    * p[0] - z50: size at which logistic function reaches 0.5
 *    * p[1] - slope
 *
 * @param zBs - DataVector with sizes at which to evaluate selectivity
 * @param p  - ParameterVector with parameter values
 * @param consts - DataVector
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *  * consts[2] - z_ref: reference size (also indicates option for normalization)
 *
 * S(z) = 0                                      : if z < z_min or z_max < z
 * S(z) = 1/(1+exp(-p[1]*(z-p[0]))),             : z otherwise
 * if (z_ref>0) S_r = 1+exp(-p[1]*(z_ref-p[0])); : normalize to value at fsz
 * if (z_ref<0) S_r = max(S(z));                 : normalize to max value
 * if (z_ref=0) S_r = 1.0;                       : no normalization
 * S(z) *= S_r;
 */
template <class Type>
  VariableVector<Type> ascLogisticSel1(DataVector<real_t> zBs,
                                       ParameterVector<Type> p,
                                       DataVector<real_t> consts){
    VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    real_t fsz   = consts[2];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    Type nS = Type(1);//normalization constant
    if (fsz>0) nS = Type(1)+exp(-p[1]*(fsz-p[0]));
    if (fsz<0) nS = Type(1)+exp(-p[1]*(gmacs::last(zBs)-p[0]));
    for (int i=0;i<zBs.size();i++) {
      S[i] = w[i]*nS/(Type(1)+exp(-p[1]*(zBs[i]-p[0])));
    }
    return S;
  }//--ascLogisticSel1 function
/**
 * ascLogisticSel2: Returns a differentiable vector representing selectivity that varies
 * across a range of sizes based on an ascending logistic curve with parameters
 *    * p[0] - z50: size at which logistic function reaches 0.50
 *    * p[1] - z95: size at which logistic function reaches 0.95
 *
 * @param zBs - DataVector with sizes at which to evaluate selectivity
 * @param p - ParameterVector with parameter values
 * @param consts - DataVector
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *  * consts[2] - z_ref: reference size (also indicates option for normalization)
 *
 * S(z) = 0                                               : if z < z_min or z_max < z
 * S(z) = 1/(1+exp(-log(19)*(z-p[0])/(p[1]-p[0])))        : z otherwise
 * if (z_ref>0) S_r = 1+exp(-log(19)*(z_ref-p[0])/(p[1]-p[0])); : normalize to value at fsz
 * if (z_ref<0) S_r = max(S(z));                          : normalize to max value
 * if (z_ref=0) S_r = 1.0;                                : no normalization
 * S(z) *= S_r;
 */
template <class Type>
  VariableVector<Type> ascLogisticSel2(DataVector<real_t> zBs,
                                       ParameterVector<Type> p,
                                       DataVector<real_t> consts){
    VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    real_t fsz   = consts[2];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    Type ln19 = log(Type(19.0));
    Type d95  = p[1]-p[0];//increment from z50 to z95
    Type nS   = Type(1);//normalization constant
    if (fsz>0) nS = Type(1)+exp(-ln19*(fsz-p[0])/d95);
    if (fsz<0) nS = Type(1)+exp(-ln19*(gmacs::last(zBs)-p[0])/d95);
    for (int i=0;i<zBs.size();i++){
      S[i] = w[i]*nS/(Type(1)+exp(-ln19*(zBs[i]-p[0])/d95));
    }
    return S;
  }//--ascLogisticSel2 function
/**
 * ascNormalSel1: Returns a differentiable vector representing selectivity that varies
 * across a range of sizes based on an ascending normal curve with parameters
 *    * p[0] - size at which ascending limb reaches 1.0
 *    * p[1] - width (std. dev) of ascending limb
 *
 * @param zBs - DataVector with sizes at which to evaluate selectivity
 * @param p   - ParameterVector with parameter values
 * @param consts - DataVector
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *
 * S(z) = 0                                         : if z < z_min or z_max < z
 * S(z) = exp(-Type(0.5)*square((z-ascMnZ)/ascWdZ)) : otherwise if z <= p[0]
 * S(z) = 1                                         : otherwise if z > p[0]
 */
template <class Type>
  VariableVector<Type> ascNormalSel1(DataVector<real_t> zBs,
                                     ParameterVector<Type> p,
                                     DataVector<real_t> consts){
    VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    Type slp = Type(5.0);
    Type ascMnZ = p(0);//size at which ascending limb hits 1
    Type ascWdZ = p(1);//width of ascending limb
    //Rcpp::Rcout<<ascMnZ<<"  "<<ascWdZ<<std::endl;
    for (int i=0;i<zBs.size();i++) {
      Type ascN = exp(-Type(0.5)*gmacs::square((zBs[i]-ascMnZ)/ascWdZ));
      Type ascJ = Type(1.0)/(Type(1.0)+exp(slp*(zBs[i]-ascMnZ)));
      S[i] = ascJ*ascN + (Type(1.0)-ascJ);
      S[i] *= w[i];
      // Rcpp::Rcout<<zBs[i]<<"  "<<ascN<<"  "<<ascJ<<"  "<<S[i]<<std::endl;
    }
    return S;
  }//--ascNormalSel1 function
/**
 * dblNormalSel1: Returns a differentiable vector representing selectivity that varies
 * across a range of sizes based on an double normal curve with parameters
 *    * p[0] - ascMnZ: size at which ascending limb reaches 1.0
 *    * p[1] - ascWdZ: width (std. dev) of ascending limb
 *    * p[2] - offset to size at which descending limb departs from 1 (dscMnZ = ascMnZ+p[2])
 *    * p[3]-  dscWdZ: width of descending limb
 *
 * @param zBs - DataVector with sizes at which to evaluate selectivity
 * @param p   - ParameterVector with parameter values
 * @param consts - DataVector
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *
 * S(z) = exp(-Type(0.5)*square((z-ascMnZ)/ascWdZ)) : if z <= p[0]
 * S(z) = 1                                         : if p[0] < z < p[0]+p[3]
 * S(z) = exp(-Type(0.5)*square((z-dscMnZ)/dscWdZ)) : if p[0]+p[3] <= z
 * S(z) = 0                                         : if z < z_min or z_max < z
 */
template <class Type>
  VariableVector<Type> dblNormalSel1(DataVector<real_t> zBs,
                                     ParameterVector<Type> p,
                                     DataVector<real_t> consts){
    VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    Type slp = Type(5.0);
    Type ascMnZ = p(0);//size at which ascending limb hits 1
    Type ascWdZ = p(1);//width of ascending limb
    Type dscMnZ = p(0)+p(2);//size at which descending limb departs from 1
    Type dscWdZ = p(3);//width of descending limb
    for (int i=0;i<zBs.size();i++) {
      Type ascN = exp(-Type(0.5)*gmacs::square((zBs[i]-ascMnZ)/ascWdZ));
      Type ascJ = Type(1.0)/(Type(1.0)+exp(slp*(zBs[i]-ascMnZ)));
      Type dscN = exp(-0.5*gmacs::square((zBs[i]-dscMnZ)/dscWdZ));
      Type dscJ = Type(1.0)/(Type(1.0)+exp(-slp*(zBs[i]-dscMnZ)));
      S[i]  = (ascJ*ascN + (Type(1.0)-ascJ))*(dscJ*dscN + (Type(1.0)-dscJ));
      S[i] *= w[i];
      // Rcpp::Rcout<<zBs[i]<<"  "<<ascN<<"  "<<ascJ<<"  "<<S[i]<<std::endl;
    }
    return S;
  }//--dblNormalSel1 function
/**
 * splineSel1: Returns a differentiable vector representing selectivity that varies
 * across a range of sizes based on a spline curve with parameters given as
 * {knot,value} pairs, where value is on the arithmetic scale but is converted to
 * the logistic scale to determine the spline. The tmbutils::splinefun class is
 * used to calculate the spline function.
 *
 * @param zBs - DataVector with sizes at which to evaluate selectivity
 * @param p   - ParameterVector with parameter values
 * @param consts - DataVector
 *  * consts[0] - z_min: minimum size with non-zero selectivity
 *  * consts[1] - z_max: maximum size with non-zero selectivity
 *
 */
template <class Type>
  VariableVector<Type> splineSel1(DataVector<real_t> zBs,
                                     ParameterVector<Type> p,
                                     DataVector<real_t> consts){
    //VariableVector<Type> S = VariableVector<Type>(zBs.size());
    real_t z_min = consts[0];
    real_t z_max = consts[1];
    DataVector<real_t> w = gmacs::window<real_t>(zBs,z_min,z_max);
    int np = p.size()/2;//should be even (knot + value at knot pairs)
    VariableVector<Type> knots = VariableVector<Type>(np);
    VariableVector<Type> kvals = VariableVector<Type>(np);
    for (int i=0;i<np;i++){
      knots[i] = p[2*i];
      kvals[i] = p[2*i+1];
      kvals[i] = log(kvals[i]/(Type(1)-kvals[i]));
    }
    tmbutils::splinefun<Type> spfun = tmbutils::splinefun<Type>(knots,kvals,2);//2: natural spline; 3: fmm spline
    VariableVector<Type> S = spfun(zBs);
    for (int i=0;i<zBs.size();i++) {
      S[i] = w[i]/(Type(1)+exp(-S[i]));
      // Rcpp::Rcout<<zBs[i]<<"  "<<ascN<<"  "<<ascJ<<"  "<<S[i]<<std::endl;
    }
    return S;
  }//--splineSel1 function
}//--gmacs namespace
#endif //--GMACS_TMB_SELECTIVITYFUNCTIONS_HPP
