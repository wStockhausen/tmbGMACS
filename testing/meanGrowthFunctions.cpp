/** \file meanGrowthFunctions.cpp
 */
/**
 * File: meanGrowthFunctions.cpp
 *
 * Create an Rcpp module when compiled using Rcpp::sourceCPP(...)
 * to be able to test mean growth functions from R.
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

#include <Rcpp.h>
#include "../inst/include/tmb/MeanGrowthFunctions.hpp"

#ifdef TMB_MODEL
using Type = double;
#endif

RCPP_MODULE(mngrwfcns_module) {
  Rcpp::function("mnGrw1",    &gmacs::mnGrw1<Type>,   "mean growth function 1");
  Rcpp::function("mnGrw2",    &gmacs::mnGrw2<Type>,   "mean growth function 2");
  Rcpp::function("mnGrw3",    &gmacs::mnGrw3<Type>,   "mean growth function 3");
}//--mngrwfcns_module
