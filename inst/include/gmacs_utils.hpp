//miscellaneous helper routines
#ifndef _HPP_GMACS_UTILS_
#define _HPP_GMACS_UTILS_

#include <iostream>

/**
 * macro to print name and value of object to terminal
 */
#define see(object) std::cout << #object ":\n" << object << "\n";

/**
 * Template function to check if input is NA
 */
template<class Type>
bool isNA(Type x){return R_IsNA(asDouble(x));}

#endif
