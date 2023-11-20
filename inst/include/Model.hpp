/**
* File: Model.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * GMACS is modeled after, and heavily indebted to, the NMFS FIMS project.
 * See LICENSE in the source folder for reuse information.
 *
*/

#ifndef GMACS_MODEL_HPP
#define GMACS_MODEL_HPP

#include <memory>

namespace gmacs{
/**
 *
 */
  template <class Type>
  class Model{
    public:
      /** Pointer to singleton instance of the gmacs model.
       * Access this instance using gmacs::Model<Type>::get_instance();
      */
      static gmacs::Model<Type> p_gmacs_model;
#ifdef TMB_MODEL
      /** pointer to TMB objective function */
      ::objective_function<Type> *p_objective_function;
#endif

    /**
     * Returns a single Information object for type Type.
     *
     * @return singleton for type Type
     */
    static std::shared_ptr<Model<Type> > get_instance() {
      if (Model<Type>::p_gmacs_model == nullptr) {
        Model<Type>::p_gmacs_model = std::make_shared<gmacs::Model<Type>>();
      }
      return Model<Type>::p_gmacs_model;
    }

    /**
     * Constructor.
     */
    Model<Type>(){
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
std::shared_ptr<Model<Type>> p_gmacs_model = nullptr;

}  // namespace gmacs

#endif /* GMACS_MODEL_HPP */
