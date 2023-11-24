/** \file Dimensions.hpp
 */
/**
* File: Dimensions.hpp
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

#ifndef GMACS_DIMENSIONS_HPP
#define GMACS_DIMENSIONS_HPP

#include <Rcpp.h>

namespace gmacs{
  class dimensions{
    public:
      int ndms = 0;
      std::vector<std::string> nms;
      Rcpp::
  };
}//--namespace gmacs

#endif //--GMACS_DIMENSIONS_HPP
