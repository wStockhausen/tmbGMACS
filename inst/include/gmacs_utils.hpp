//miscellaneous helper routines
#ifndef _HPP_GMACS_UTILS_
#define _HPP_GMACS_UTILS_

#include <iostream>
#include "tmbGMACS_types.hpp"

/**
 * macro to print name and value of object to terminal
 */
#define see(object) std::cout << #object ":\n" << object << "\n";

/**
 * Template function to check if input is NA
 */
template<class Type>
bool isNA(Type x){return R_IsNA(asDouble(x));}

namespace gmacs{
  /**
   * function to return final value of an Eigen vector
   */
  template<class Type>
  Type last(Eigen::Matrix<Type,Eigen::Dynamic,1,Eigen::ColMajor> v){
    return v[v.size()-1];
  }
  /**
   * function to return square of value
   */
  template<class Type>
  Type square(Type v){
    return v*v;
  }
  /**
   * Returns a vector representing a window (square wave) with value 1 from z_min to
   * z_max and 0 otherwise
   *
   * @param x - DataVector with values at which to evaluate the square wave
   * @param min - value of independent variable at which square wave turns "on"
   * @param max - value of independent variable at which square wave turns "off"
   *
   * S(zBs) = p[0];
   */
  template<class Type>
  DataVector<Type> window(DataVector<Type> x, Type min, Type max){
    DataVector<Type> y = DataVector<Type>(x.size());
    for (int i=0;i<x.size();i++) {
      Type ascJ = Type(1.0)/(Type(1.0)+exp(+Type(5)*(x[i]-min)));
      Type dscJ = Type(1.0)/(Type(1.0)+exp(-Type(5)*(x[i]-max)));
      y[i] = ((1-ascJ)*(1-dscJ));
    }
    return y;
  }//--window function
#ifdef TMB_MODEL
  /**
   * function to return final value of a CppAD vector
   */
  template<class Type>
  Type last(CppAD::vector<Type> v){
    return v[v.size()-1];
  }
#endif //--ifdef TMB_MODEL
}//--namespace gmacs
#endif //--_HPP_GMACS_UTILS_
