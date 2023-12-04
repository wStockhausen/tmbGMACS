/** \file PopProjector.hpp
 */
/**
* File: PopProjector.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * GMACS is modeled after, and heavily indebted to, the NMFS FIMS project.
 * See LICENSE in the source folder for reuse information.
 *
*/

#ifndef GMACS_POPPROJECTOR_HPP
#define GMACS_POPPROJECTOR_HPP

#include <iostream>
#include <Rcpp.h>
#include "Dimensions.hpp"

namespace gmacs{
  class PopProjector{
    public:
      Dimensions dims;
      Rcpp::NumericVector pop;

      PopProjector(){}

      Rcpp::NumericVector getPop(){
        Rcpp::NumericVector pop_ = clone(pop);
        return pop_;
      }

      void setPop(Rcpp::NumericVector pop_){
        pop = pop_;
      }
  };//--PopProjector
} //--gmacs namespace

#endif //GMACS_POPPROJECTOR_HPP

