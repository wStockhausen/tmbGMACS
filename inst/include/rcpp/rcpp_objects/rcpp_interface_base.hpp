/*
 * File:   rcpp_interface_base.hpp
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Fisheries Integrated Modeling System project. See LICENSE file
 * for reuse information.
 *
 */
#ifndef GMACS_RCPP_RCPP_OBJECTS_RCPP_INTERFACE_BASE_HPP
#define GMACS_RCPP_RCPP_OBJECTS_RCPP_INTERFACE_BASE_HPP

#include <map>
#include <vector>

#include "../../../common/def.hpp"
#include "../../../common/information.hpp"
#include "../../interface.hpp"

#define RCPP_NO_SUGAR
#include <Rcpp.h>

/**
 *@brief Base class for all interface objects
 */
class GMACSRcppInterfaceBase {
 public:
  /**< GMACS interface object vectors */
  static std::vector<GMACSRcppInterfaceBase *> gmacs_interface_objects;

  /** @brief virtual method to inherit to add objects to the TMB model */
  virtual bool add_to_gmacs_tmb() {
    std::cout << "gmacs_rcpp_interface_base::add_to_gmacs_tmb(): Not yet "
                 "implemented.\n";
    return false;
  }
};
std::vector<GMACSRcppInterfaceBase *>
    GMACSRcppInterfaceBase::gmacs_interface_objects;

#endif //GMACS_RCPP_RCPP_OBJECTS_RCPP_INTERFACE_BASE_HPP
