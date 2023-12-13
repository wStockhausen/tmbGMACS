/** \file tmbGMACS_types.hpp
 * This file includes macro and alias definitions.
*/
/**
 * File: init.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
 *
 * This file is basically copied from FIMS include/interface/init.h, with
 * "FIMS" replaced by "GMACS" (using similar case).
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * GMACS was originally developed using the AD Model Builder (ADMB) modeling
 * framework.
 *
 * This TMB version of GMACS is heavily indebted to the NOAA/NMFS Fisheries
 * Integrated Model System (FIMS) project for its framework and code.
 *
 * See LICENSE in the source folder for reuse information.
 *
*/

#ifndef INTERFACE_INIT_HPP
#define INTERFACE_INIT_HPP
#include <R_ext/Rdynload.h>
#include <stdlib.h>

/**
 *
 * Callback definition for TMB C++ functions.
 *
 * include/tmb_core.hpp also defines TMB_CALLDEFS and the following functions,
 * but also includes "tmbad_print", "TransformADFunObject", "getFramework",
 * and "getSetGlobalPtr" function callbacks, which are missing here.
 *
 */
#ifndef TMB_CALLDEFS
#define TMB_CALLDEFS  \
      {"MakeADFunObject",     (DL_FUNC)&MakeADFunObject, 4},     \
      {"InfoADFunObject",     (DL_FUNC)&InfoADFunObject, 1},     \
      {"EvalADFunObject",     (DL_FUNC)&EvalADFunObject, 3},     \
      {"MakeDoubleFunObject", (DL_FUNC)&MakeDoubleFunObject, 3}, \
      {"EvalDoubleFunObject", (DL_FUNC)&EvalDoubleFunObject, 3}, \
      {"getParameterOrder",   (DL_FUNC)&getParameterOrder, 3},   \
      {"MakeADGradObject",    (DL_FUNC)&MakeADGradObject, 3},    \
      {"MakeADHessObject2",   (DL_FUNC)&MakeADHessObject2, 4},   \
      {"usingAtomics",        (DL_FUNC)&usingAtomics, 0},        \
      {"TMBconfig",           (DL_FUNC)&TMBconfig, 2}
#endif

#define CALLDEF(name, n) \
  { #name, (DL_FUNC)&name, n }

extern "C" {

SEXP compois_calc_var(SEXP mean, SEXP nu);
SEXP omp_check();
SEXP omp_num_threads(SEXP);
SEXP _rcpp_module_boot_tmbgmacs();

/**
 * Callback definition to load the GMACS module.
 *
 * TMB defines something in a similar, but not identical, manner.
 * See #ifdef TMB_LIB_INIT section in include/tmb_core.hpp
 *
 */
//static const R_CallMethodDef CallEntries[] = {
static R_CallMethodDef CallEntries[] = {
    TMB_CALLDEFS,
    {"_rcpp_module_boot_tmbgmacs", (DL_FUNC)&_rcpp_module_boot_tmbgmacs, 0},
    {NULL, NULL, 0}};

/**
 * GMACS shared object initializer.
 * Note that the macro variable TMB_LIB_INIT is
 * #define'd to be R_init_tmbGMACS_TMBExports in tmbGMACS_TMBExports.cpp
 *
 * @param dll
 *
 */
void R_init_tmbGMACS_TMBExports(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
#ifdef TMB_CCALLABLES
  TMB_CCALLABLES("tmbGMACS");
#endif
}
}

#endif
