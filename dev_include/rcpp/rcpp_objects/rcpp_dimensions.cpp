/*
 * File:   rcpp_dimensions.hpp
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Fisheries Integrated Modeling System project. See LICENSE file
 * for reuse information.
 *
 */
#ifndef GMACS_RCPP_RCPP_OBJECTS_RCPP_DIMENSIONS_HPP
#define GMACS_RCPP_RCPP_OBJECTS_RCPP_DIMENSIONS_HPP

#include <map>
#include <vector>

#include "../../../common/def.hpp"
#include "../../../common/information.hpp"
#include "../../interface.hpp"

#define RCPP_NO_SUGAR
#include <Rcpp.h>

/**
 * @brief RcppInterface class that defines
 * the interface between R and C++ for model dimensions.
 */
class Dimensions {
 public:
   /** initial value of the parameter*/
  double value_m;

  /**
   * @brief Constructor for initializing Parameter.
   * @details Inputs: value, min, max, is_estimated.
   */
  Dimensions(double value, double min, double max, bool is_estimated)
      : value_m(value), min_m(min), max_m(max), is_estimated_m(estimated) {}

  /**
   * @brief No-value constructor for initializing Parameter.
   * @details Set value to 0 when there is no input value.
   */
  Dimensions() { value_m = 0; }

  getIndex(){}

  /**
   *
   */
  getIndices(int i1d){

  }
};

#endif //GMACS_RCPP_RCPP_OBJECTS_RCPP_DIMENSIONS_HPP
