
//#define TMB_MODEL //def/undef for testing only (move to R as compile flag?)
#include <Rcpp.h>
#include "../inst/include/tmb/SelectivityFunctions.hpp"

#ifdef TMB_MODEL
using Type = double;
#endif

RCPP_MODULE(selfcns_module) {
  Rcpp::function("window",         &gmacs::window<Type>,         "window function");
  Rcpp::function("constantSel",    &gmacs::constantSel<Type>,    "constant size dependence");
  Rcpp::function("ascLogisticSel1",&gmacs::ascLogisticSel1<Type>,"ascending logistic with parameters z50 and slope");
  Rcpp::function("ascLogisticSel2",&gmacs::ascLogisticSel2<Type>,"ascending logistic with parameters z50 and z95");
  Rcpp::function("ascNormalSel1",  &gmacs::ascNormalSel1<Type>,  "ascending normal with parameters z_peak and width");
  Rcpp::function("dblNormalSel1",  &gmacs::dblNormalSel1<Type>,  "double normal with parameters ascending z_peak and width, offset, descending width");
  Rcpp::function("splineSel1",     &gmacs::splineSel1<Type>,     "regression spline with knots (fixed parameters) and values-at-knots (estimated parameters)");
}//--m_module
