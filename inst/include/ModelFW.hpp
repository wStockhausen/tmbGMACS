/** \file ModelFW.hpp
 */
/**
 * File: ModelFW.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
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

#ifndef GMACS_MODELFW_HPP
#define GMACS_MODELFW_HPP

#include <memory>

namespace gmacs{
/**
 * ModelFW: GMACS model framework class
 */
  template <class Type>
  class ModelFW{
    public:
      /** Pointer to singleton instance of the gmacs model framework.
       * Access this instance using gmacs::ModelFW<Type>::get_instance();
      */
      static gmacs::ModelFW<Type> p_gmacs_modelFW;
#ifdef TMB_MODEL
      /** pointer to TMB objective function */
      ::objective_function<Type> *p_objective_function;
#endif
      ParameterFunctions* p_params;

    /**
     * Returns a single Information object for type Type.
     *
     * @return singleton for type Type
     */
    static std::shared_ptr<ModelFW<Type> > get_instance() {
      if (ModelFW<Type>::p_gmacs_modelFW == nullptr) {
        ModelFW<Type>::p_gmacs_modelFW = std::make_shared<gmacs::ModelFW<Type> >();
      }
      return ModelFW<Type>::p_gmacs_modelFW;
    }

    /**
     * Constructor.
     */
    ModelFW<Type>(){
      p_params = nullptr;
#ifdef TMB_MODEL
      p_objective_function = nullptr;
#endif
    };

    /**
     * @brief evaluate. Runs the model and calculates the joint negative log-likelihood function.
     *
     * TODO: complete this!
     */
    const Type evaluate(){
      Type jnll = Type(0); //!< dummy return value for now (TODO)
      return(jnll);
    }
  };

/**
 * This is the singleton instance of the Model class.
 *
 * Access this instance using the static class method
 * gmacs::Model<Type>
 */
template <typename Type>
std::shared_ptr<ModelFW<Type>> p_gmacs_modelFW = nullptr;

}  // namespace gmacs

#endif /* GMACS_MODELFW_HPP */
