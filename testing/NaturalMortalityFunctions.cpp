
//#define TMB_MODEL //def/undef for testing only (move to R as compile flag?)
#include <Rcpp.h>
#include "../inst/include/tmb/NaturalMortalityFunctions.hpp"

#ifdef TMB_MODEL
using Type = double;
#endif

RCPP_MODULE(m_module) {
  Rcpp::function("constantM",&gmacs::constantM<Type>,"constant size dependence");
  Rcpp::function("lorenzenM",&gmacs::lorenzenM<Type>,"Lorenzen size dependence");
}//--m_module
