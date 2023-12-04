
//#define TMB_MODEL //def/undef for testing only (move to R as compile flag?)
#include <Rcpp.h>
#include "../inst/include/tmb/GrowthFunctions.hpp"

#ifdef TMB_MODEL
using Type = double;
#endif

RCPP_MODULE(grwfcns_module) {
  Rcpp::function("mnGrw1",    &gmacs::mnGrw1<Type>,   "mean growth function 1");
  Rcpp::function("mnGrw2",    &gmacs::mnGrw2<Type>,   "mean growth function 2");
  Rcpp::function("mnGrw3",    &gmacs::mnGrw3<Type>,   "mean growth function 3");
//  Rcpp::function("probGrw1",  &gmacs::probGrw1<Type>, "growth probability function 1");
}//--grwfcns_module
