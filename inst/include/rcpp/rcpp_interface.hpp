/*
 * File:   rcpp_interface.hpp
 *
 *
 * This file is part of the NOAA, National Marine Fisheries Service (NMFS)
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * It relies heavily on development by NMFS personnel on the NMFS
 * Fishery Information Model System (FIMS).
 * See LICENSE file for reuse information.
 *
 *
 */
#ifndef GMACS_INTERFACE_RCPP_INTERFACE_HPP
#define GMACS_INTERFACE_RCPP_INTERFACE_HPP

#include "rcpp_objects/rcpp_Parameter.hpp"

/**
 * @brief Create the TMB model object and add interface objects to it.
 */
bool CreateTMBModel() {
  for (size_t i = 0; i < GMACSRcppInterfaceBase::gmacs_interface_objects.size();
       i++) {
    GMACSRcppInterfaceBase::gmacs_interface_objects[i]->add_to_gmacs_tmb();
  }

  // base model
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_REAL_TYPE>> d0 =
      gmacs_info::Information<TMB_GMACS_REAL_TYPE>::GetInstance();
  d0->CreateModel();

  // first-order derivative
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_FIRST_ORDER>> d1 =
      gmacs_info::Information<TMB_GMACS_FIRST_ORDER>::GetInstance();
  d1->CreateModel();

  // second-order derivative
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_SECOND_ORDER>> d2 =
      gmacs_info::Information<TMB_GMACS_SECOND_ORDER>::GetInstance();
  d2->CreateModel();

  // third-order derivative
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_THIRD_ORDER>> d3 =
      gmacs_info::Information<TMB_GMACS_THIRD_ORDER>::GetInstance();
  d3->CreateModel();

  return true;
}

Rcpp::NumericVector get_fixed_parameters_vector() {
  // base model
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_REAL_TYPE>> d0 =
      gmacs_info::Information<TMB_GMACS_REAL_TYPE>::GetInstance();

  Rcpp::NumericVector p;

  for (size_t i = 0; i < d0->fixed_effects_parameters.size(); i++) {
    p.push_back(*d0->fixed_effects_parameters[i]);
  }

  return p;
}

Rcpp::NumericVector get_random_parameters_vector() {
  // base model
  std::shared_ptr<gmacs_info::Information<TMB_GMACS_REAL_TYPE>> d0 =
      gmacs_info::Information<TMB_GMACS_REAL_TYPE>::GetInstance();

  Rcpp::NumericVector p;

  for (size_t i = 0; i < d0->random_effects_parameters.size(); i++) {
    p.push_back(*d0->random_effects_parameters[i]);
  }

  return p;
}


#endif //GMACS_INTERFACE_RCPP_INTERFACE_HPP
