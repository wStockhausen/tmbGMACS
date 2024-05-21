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
#ifndef GMACS_RCPP_INTERFACE_HPP
#define GMACS_RCPP_INTERFACE_HPP

#ifndef RCPP_NO_SUGAR
  #define RCPP_NO_SUGAR
  #include <Rcpp.h>
#endif
//#include "rcpp_objects/rcpp_Parameter.hpp"
#include "rcpp_objects/rcpp_dimensions.hpp"

using namespace Rcpp;

/**
 * @brief Create the TMB model object and add interface objects to it.
 */
bool CreateTMBModel(){return false;}

/**
 * Get the values of the fixed parameters as a vector
 */
Rcpp::NumericVector get_fixed_parameters_vector() {

  Rcpp::NumericVector p;

  // for (size_t i = 0; i < d0->fixed_effects_parameters.size(); i++) {
  //   p.push_back(*d0->fixed_effects_parameters[i]);
  // }

  return p;
}

/**
 * Get the values of the random parameters as a vector
 */
Rcpp::NumericVector get_random_parameters_vector() {

  Rcpp::NumericVector p;

  // for (size_t i = 0; i < d0->random_effects_parameters.size(); i++) {
  //   p.push_back(*d0->random_effects_parameters[i]);
  // }

  return p;
}

//RCPP_EXPOSED_CLASS(Parameter)
RCPP_EXPOSED_CLASS(Dimensions)

RCPP_MODULE(tmbgmacs) {
  Rcpp::function("CreateTMBModel", &CreateTMBModel);
  Rcpp::function("get_fixed_parameters", &get_fixed_parameters_vector);
  Rcpp::function("get_random_parameters", &get_random_parameters_vector);

  // Rcpp::class_<Parameter>("Parameter")
  //     .constructor()
  //     .constructor<double>()
  //     .constructor<Parameter>()
  //     .field("value", &Parameter::value_m)
  //     .field("min", &Parameter::min_m)
  //     .field("max", &Parameter::max_m)
  //     .field("is_random_effect", &Parameter::is_random_effect_m)
  //     .field("estimated", &Parameter::estimated_m);
  Rcpp::class_<gmacs::Dimensions>("Dimensions")
      .constructor<Rcpp::CharacterVector,
                   Rcpp::List,
                   Rcpp::IntegerMatrix,
                   Rcpp::IntegerVector>()
      .field("numDims",    &gmacs::Dimensions::num_dims)
      .field("dimsNames",  &gmacs::Dimensions::dims_names)
      .field("dimsNums",   &gmacs::Dimensions::dims_nums)
      .field("dimsLevels", &gmacs::Dimensions::dims_lvls)
      .field("mapS2D",     &gmacs::Dimensions::mapS2D)
      .field("mapD2S",     &gmacs::Dimensions::mapD2S)
      //int getSparseIndex(Rcpp::IntegerVector lvls,bool zerosBased=true){
      .method("getSparseIndex", &gmacs::Dimensions::getSparseIndex)
      //int getDenseIndex(Rcpp::IntegerVector lvls,bool zerosBased=true){
      .method("getDenseIndex",  &gmacs::Dimensions::getDenseIndex)
      //Rcpp::CharacterVector getDimensionsLevelsFromSparseIndex(int idx, bool zerosBased=true){
      .method("getDimensionsLevelsFromSparseIndex",       &gmacs::Dimensions::getDimensionsLevelsFromSparseIndex)
      //Rcpp::CharacterVector getDimensionsLevelsFromDenseIndex(int idx, bool zerosBased=true){
      .method("getDimensionsLevelsFromDenseIndex",        &gmacs::Dimensions::getDimensionsLevelsFromDenseIndex)
      //Rcpp::IntegerVector getDimensionsLevelIndicesFromSparseIndex(int idx, bool zerosBased=true){
      .method("getDimensionsLevelIndicesFromSparseIndex", &gmacs::Dimensions::getDimensionsLevelIndicesFromSparseIndex)
      //Rcpp::IntegerVector getDimensionsLevelIndicesFromDenseIndex(int idx, bool zerosBased=true){
      .method("getDimensionsLevelIndicesFromDenseIndex",  &gmacs::Dimensions::getDimensionsLevelIndicesFromDenseIndex);
} //--RCPP_MODULE(tmbgmacs)

#endif //--GMACS_RCPP_INTERFACE_HPP

