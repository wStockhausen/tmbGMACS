// __DO NOT__ '#include <TMB.hpp>' as file is not include-guarded
//#include <TMB.hpp> //--comment out before compiling for package construction
#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR obj

#include "../../include/Model.hpp"

// name of function _must_ match file name (gmacs)
template<class Type>
Type gmacs(objective_function<Type>* obj) {
  PARAMETER_VECTOR(p);

  std::shared_ptr<gmacs::Model<Type>> model = gmacs::Model<Type>::getInstance();

  /** update the model parameter values using the values from p */
  //TODO: fill this in!

  /** assign the gmacs objective function pointer to obj (or this?) */
  model->p_objective_function = obj;

  Type objfun = model.evaluate();

  return objfun;
}


#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR this
