/** \file Population.hpp
 */
/**
* File: Population.hpp
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

#ifndef GMACS_POPULATION_HPP
#define GMACS_POPULATION_HPP

#include <iostream>
#include <Rcpp.h>
#include "Dimensions.hpp"
#include "tmbGMACS_types.hpp"

namespace gmacs{
  template <class Type>
  class Population<Type>{
    public:
      Dimensions dims;
      Eigen::Matrix<Type pop;

      Population(){}

      Rcpp::NumericVector getPop(){
        Rcpp::NumericVector pop_ = clone(pop);
        return pop_;
      }

      void setPop(Rcpp::NumericVector pop_){
        pop = pop_;
      }
  };//--Population
} //--gmacs namespace

#endif //GMACS_POPULATION_HPP

