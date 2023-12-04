#include <Rcpp.h>
#include "../inst/include/Dimensions.hpp"

RCPP_MODULE(dims_module) {
  Rcpp::class_<gmacs::Dimensions>("Dimensions")
    .constructor<Rcpp::CharacterVector,Rcpp::List,
                 Rcpp::IntegerMatrix,Rcpp::IntegerVector>()
    .field("mapS2D", &gmacs::Dimensions::mapS2D)
    .field("mapD2S", &gmacs::Dimensions::mapD2S)
    .method("getSparseIndex", &gmacs::Dimensions::getSparseIndex)
    .method("getDenseIndex", &gmacs::Dimensions::getDenseIndex)
    .method("getDimensionsLevelsFromSparseIndex",&gmacs::Dimensions::getDimensionsLevelsFromSparseIndex,
                 "get vector of dimension levels corresponding to the sparse index value")
    .method("getDimensionsLevelsFromDenseIndex",&gmacs::Dimensions::getDimensionsLevelsFromDenseIndex,
                 "get vector of dimension levels corresponding to the dense index value")
    .method("getDimensionsLevelIndicesFromSparseIndex",&gmacs::Dimensions::getDimensionsLevelsFromSparseIndex,
                 "get vector of indices for dimension levels corresponding to the sparse index value")
    .method("getDimensionsLevelIndicesFromDenseIndex",&gmacs::Dimensions::getDimensionsLevelsFromDenseIndex,
                 "get vector of indices for dimension levels corresponding to the dense index value")
    ;
}
