/*
 * File:   rcpp_parameter.hpp
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Fisheries Integrated Modeling System project. See LICENSE file
 * for reuse information.
 *
 */
#ifndef GMACS_RCPP_RCPP_OBJECTS_RCPP_PARAMETER_HPP
#define GMACS_RCPP_RCPP_OBJECTS_RCPP_PARAMETER_HPP

#include <map>
#include <vector>

#include "../../../common/def.hpp"
#include "../../../common/information.hpp"
#include "../../interface.hpp"

#define RCPP_NO_SUGAR
#include <Rcpp.h>

/**
 * @brief RcppInterface class that defines
 * the interface between R and C++ for parameters.
 */
class Parameter {
 public:
   /**< initial value of the parameter*/
  double value_m;
    /**< min value of the parameter*/
  double min_m = std::numeric_limits<double>::min();
  /**< max value of the parameter*/
  double max_m = std::numeric_limits<double>::max();
  /**< Is the parameter estimated? Default value is false.*/
  bool is_estimated_m = false;
  /**< Is the parameter a random effect parameter? Default value is false.*/
  bool is_random_effect_m = false;
  /**< prior type. Default is none.*/
  int prior_type_m = 0;
  /**< prior location value.*/
  double p1_m = 0;
  /**< prior scale value.*/
  double p2_m = 0;

  /**
   * @brief Constructor for initializing Parameter.
   * @details Inputs: value, min, max, is_estimated.
   */
  Parameter(double value, double min, double max, bool is_estimated)
      : value_m(value), min_m(min), max_m(max), is_estimated_m(estimated) {}

  /**
   * @brief Constructor for initializing Parameter.
   * @details Inputs: value, min, max, is_estimated, is_random_effect.
   */
  Parameter(double value, double min, double max, bool is_estimated,
            bool is_random_effect)
      : value_m(value), min_m(min), max_m(max), is_estimated_m(estimated),
        is_random_effect_m(is_random_effect){}

  /**
   * @brief Constructor for initializing Parameter.
   * @details Inputs: value, min, max, is_estimated, is_random_effect,
   *                  prior_type,p1,p2.
   */
  Parameter(double value, double min, double max, bool is_estimated,
            bool is_random_effect,int prior_type,double p1,double p2)
      : value_m(value), min_m(min), max_m(max),
        is_estimated_m(estimated),is_random_effect_m(is_random_effect),
        prior_type_m(prior_type),p1_m(p1),p2_m(p2){}

  /**
   * @brief Constructor for initializing Parameter.
   * @details Inputs include value.
   */
  Parameter(double value) { value_m = value; }

  /**
   * @brief Constructor for initializing Parameter.
   * @details Set value to 0 when there is no input value.
   */
  Parameter() { value_m = 0; }
};

#endif //GMACS_RCPP_RCPP_OBJECTS_RCPP_PARAMETER_HPP
