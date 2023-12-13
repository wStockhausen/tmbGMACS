/**\file test_gmacs.hpp
*/
/**
 * File: test_gmacs.hpp
 *
 * This provides access to Rcpp modules to test system components, with
 * an associated (dummy) TMB model.
 *
 * The TMB model itself does nothing but return a value of 0.
 *
 * System components to be tested can be accessed via Rcpp modules.
 *
 * The required build steps are:
 *    1. change the working directory to {pkgname}/src/TMB (here, pkgname = "tmbGMACS")
 *    2. run TMBtools::export_models(pkg=".")
 *      a. This will create the following files in the src/TMB folder:
 *        i. {pkgname}_TMBExports.cpp (the actual TMB cpp file)
 *        ii. {pkgname}_TMBExports.so (the shared library)
 *        iii. compile.R (the R file used to compile the cpp file)
 *      b. the shared library is also copied into the {pkgname}/src folder
 *    3.
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
// __DO NOT__ '#include <TMB.hpp>' as file is not include-guarded
//#include <TMB.hpp> //--comment out before compiling for package construction
#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR obj

//--TMB_LIB_INIT is defined in tmbGMACS_TMBExports.cpp (which is read-only)
//----not quite sure what side effects TMB_LIB_INIT might have so
//----using TMB_HPP_DEFINED to create guards for including TMB.hpp
#ifdef TMB_LIB_INIT
  #define TMB_HPP_DEFINED
#else
  #ifndef TMB_HPP_DEFINED
    #define TMB_HPP_DEFINED
    #include <TMB.hpp>
  #endif //--TMB_HPP_DEFINED
#endif //--TMB_LIB_INIT (defined in tmbGMACS_TMBExports.cpp)

#include "tmb/ProbGrowthFunctions.hpp"

// name of function _must_ match file name (test_gmacs)
template<class Type>
Type test_gmacs(objective_function<Type>* obj) {
  /** dummy parameter vector */
  PARAMETER_VECTOR(p);

  /** evaluate the model objective function to characterize the fit to data */
  Type objfun = 0;

  /** return the objective function to the function caller */
  return objfun;
}

#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR this
