#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR obj

//--TMB.hpp may already be included through tmbGMACS_TMBExports.cpp
//----if so, macro var TMB_HPP_DEFINED has been defined
//----check TMB_HPP_DEFINED to create guards to prevent multiple includes of TMB.hpp
#ifndef TMB_HPP_DEFINED
  #define TMB_HPP_DEFINED
  #include <TMB.hpp>
#endif //--TMB_HPP_DEFINED

#include "init.hpp"
#include "ModelFW.hpp"

// name of function _must_ match file name (GMACS)
/// @cond
/**
 * @brief TMB objective function
 *
 * @return Returns a joint negative log likelihood
 */
template<class Type>
Type GMACS(objective_function<Type>* obj) {
  PARAMETER_VECTOR(p);

  /** get pointer to model framework */
  std::shared_ptr<gmacs::ModelFW<Type>> modelFW = gmacs::ModelFW<Type>::getInstance();

  /** update the model parameter values using the values from p */
  //TODO: fill this in!
  //modelFW->updateParameters(p);

  /** assign the gmacs objective function pointer to obj (or this?) */
  modelFW->p_objective_function = obj;

  /** evaluate the model objective function to characterize the fit to data */
  Type objfun = modelFW->evaluate();

  /** TODO: simulate model? */
  /** TODO: report model results */
  /** TODO: ADreport model results? */

  /** return the objective function to the function caller */
  return objfun;
}

#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR this
