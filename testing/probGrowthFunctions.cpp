
//--want to provide probGrw1 from ProbGrowthFunctions.hpp,
//--which uses a TMB-provided function,
//--as a function available in R for testing
#define CPPAD_FRAMEWORK
#undef TMB_LIB

//--copied versions from TMB.hpp because not including TMB.hpp
template<class Type>
struct isDouble{
  enum{value=false};
};
template<>
struct isDouble<double>{
  enum{value=true};
};
//-----

#include <iostream>

#include <Rcpp.h>
Rcpp::Rostream<true>& Rcout = Rcpp::Rcout; //cppad files need Rcout

#include <cppad/CppAD.h>
template <typename Type>
  using vector = CppAD::vector<Type>;
template <typename Type>
  using matrix = Eigen::Matrix<Type,Eigen::Dynamic,Eigen::Dynamic,Eigen::ColMajor>;
using CppAD::AD;
//--from TMB.hpp to define TMB_EXTERN
#ifdef WITH_LIBTMB
#define CSKIP(...) ;
#define TMB_EXTERN extern
#else
#define CSKIP(...) __VA_ARGS__
#define TMB_EXTERN
#endif
//--
#include<Vectorize.hpp>
#include<atomic_macro.hpp>
#include<atomic_math.hpp>
#include<distributions_R.hpp>
#include "../inst/include/tmb/ProbGrowthFunctions.hpp"

#ifdef TMB_MODEL
using Type = double;
#endif

RCPP_MODULE(probgrwfcns_module) {
  Rcpp::function("probGrw1",  &gmacs::probGrw1<Type>, "growth probability function 1");
}//--probgrwfcns_module
