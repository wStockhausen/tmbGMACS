// __DO NOT__ '#include <TMB.hpp>' as file is not include-guarded
//#include <TMB.hpp> //--comment out before compiling for package construction
#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR obj

#include "../../include/ModelFW.hpp"

// name of function _must_ match file name (gmacs)
template<class Type>
Type gmacs(objective_function<Type>* obj) {
  PARAMETER_VECTOR(p);

  /** get pointer to model framework */
  std::shared_ptr<gmacs::ModelFW<Type>> modelFW = gmacs::ModelFW<Type>::getInstance();

  /** update the model parameter values using the values from p */
  //TODO: fill this in!
  modelFW->updateParameters(p);

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
