// __DO NOT__ '#include <TMB.hpp>' as file is not include-guarded
//#include <TMB.hpp> //--comment out before compiling for package construction
#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR obj

#include "../../include/gmacs_utils.hpp"

// name of function _must_ match file name (gmacs)
template<class Type>
Type gmacs(objective_function<Type>* obj) {
    DATA_STRING(mode);
    DATA_VECTOR(dims);

    PARAMETER_VECTOR(pars);

    if(mode=="debug") see(mode);

    int ndims = dims.size(); //--number of model dimensions
    REPORT(dims);

    int npars = pars.size(); //--number of  model parameters
    REPORT(npars);

    Type objfun = 0;

    return objfun;
}


#undef TMB_OBJECTIVE_PTR
#define TMB_OBJECTIVE_PTR this
