/** \file tmbGMACS.h
 * This file is a convenience "master include" file for headers.
 *
 * It should "include" all other header files (?).
 *
 * By R convention, this file should have a ".h" extension.
 */
/**
 * File: tmbGMACS.h
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

#ifndef GMACS_TMBGMACS_H
#define GMACS_TMBGMACS_H

#ifndef TMB_HPP_DEFINED
  #define TMB_HPP_DEFINED
  #include <TMB.hpp>
#endif //--TMB_HPP_DEFINED
#ifndef RCPP_NO_SUGAR
  #define RCPP_NO_SUGAR
  #include <Rcpp.h>
#endif

#include "init.hpp"
#include "rcpp/rcpp_interface.hpp"
#include "rcpp/rcpp_objects/rcpp_dimensions.hpp"
#include "ModelFW.hpp"

#endif

