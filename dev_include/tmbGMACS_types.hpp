/** \file tmbGMACS_types.hpp
 * This file includes macro and alias definitions.
*/
/**
 * File: tmbGMACS_types.hpp
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

#ifndef GMACS_TMBGMACS_TYPES_HPP
#define GMACS_TMBGMACS_TYPES_HPP


#ifndef TMB_MODEL
//Rcpp-only types---------------------------------------------------------------
#include <Rcpp.h>
#include <RcppEigen.h>

//necessary TMB includes
// #include <tmb_enable_header_only.hpp>
// #include <Rstream.hpp>
// #include <cppad/cppad.hpp>
//#include <tmb_core.hpp>

using real_t = double;
using variable_t = double;
using Type = double;
template <typename Type>
  using vector = Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor>;
template <typename Type>
  using DataVector = Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor>;
template <typename Type>
  using ParameterVector = Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor>;
template <typename Type>
  using VariableVector = Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor>;
template <typename Type>
  using VariableMatrix = Eigen::Matrix<Type,Eigen::Dynamic,Eigen::Dynamic,Eigen::ColMajor>;
template <typename Type>
  using EigenVector = Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor>;
template <typename Type>
  using EigenMatrix = Eigen::Matrix<Type,Eigen::Dynamic,Eigen::Dynamic,Eigen::ColMajor>;

#else

//TMB-only types----------------------------------------------------------------
//#include <TMB.hpp>

/** macro to define "REPORT" for an isDouble<Type>  */
#define REPORT_F(name, F)                                              \
  if (isDouble<Type>::value && F->current_parallel_region < 0) {       \
    Rf_defineVar(Rf_install(#name), PROTECT(asSEXP(name)), F->report); \
    UNPROTECT(1);                                                      \
  }


/**
  * Convert a vector<vector<Type>> (i.e., a [possibly] ragged array) object
  * to a vector suitable for ADREPORT_F'ing.
*/
template <typename Type>
vector<Type> ADREPORTvector(vector<vector<Type> > x) {
  int outer_dim = x.size();
  int dim = 0;
  for (int i = 0; i < outer_dim; i++) {
    dim += x(i).size();
  }
  vector<Type> res(dim);
  int idx = 0;
  for (int i = 0; i < outer_dim; i++) {
    int inner_dim = x(i).size();
    for (int j = 0; j < inner_dim; j++) {
      res(idx) = x(i)(j);
      idx += 1;
    }
  }
  return res;
}
/** macro to define "ADREPORT" for a vector<vector<Type>> object */
#define ADREPORT_F(name, F) F->reportvector.push(name, #name);

/** macro to define "simulate" section */
#define SIMULATE_F(F) if (isDouble<Type>::value && F->do_simulate)

/** */
#define TMB_GMACS_REAL_TYPE double
#define TMB_GMACS_FIRST_ORDER AD<TMB_FIMS_REAL_TYPE>
#define TMB_GMACS_SECOND_ORDER AD<TMB_FIMS_FIRST_ORDER>
#define TMB_GMACS_THIRD_ORDER AD<TMB_FIMS_SECOND_ORDER>

/** Alias for a scalar that is not differentiable */
  using real_t = double;
/** Alias for a scalar (that is differentiable?) */
template <typename Type>
  using variable_t = Type;
/** Alias for a data vector (that is differentiable(?)) */
template <typename Type>
  using DataVector = CppAD::vector<Type>;
/** Alias for a parameter vector that is differentiable */
template <typename Type>
  using ParameterVector = CppAD::vector<Type>;
/** Alias for a (variable) vector that is differentiable */
template <typename Type>
  using VariableVector = CppAD::vector<Type>;
/** Alias for a (variable) matrix as defined in TMB's namespace Eigen (see include/tmbutils/vector.hpp) */
template <typename Type>
  using VariableMatrix = tmbutils::matrix<Type>;
/** Alias for a (variable) vector as defined in TMB's namespace Eigen (see include/tmbutils/vector.hpp) */
template <typename Type>
  using EigenVector = tmbutils::vector<Type>;
/** Alias for a (variable) matrix as defined in TMB's namespace Eigen (see include/tmbutils/vector.hpp) */
template <typename Type>
  using EigenMatrix = tmbutils::matrix<Type>;

#endif //TMB_MODEL

#endif //GMACS_TMBGMACS_TYPES_HPP

