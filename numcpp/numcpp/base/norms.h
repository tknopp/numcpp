#ifndef NUMCPP_NORMS_H
#define NUMCPP_NORMS_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup norms
@brief Norm functions
@ingroup base
@{
*/

/*!
Return the p-norm of the input array x (considered as vector).
The parameter \a p is in the range of [0,infinity). Infinity is defined by INFINITY.
*/
template<class Array>
double norm(const Array& x, double p=2.0)
{
  if(p == 0)
    return ( sum( abs(x) != 0 ) );
  if(std::isinf(p))
    return ( max( abs(x)) );

  return std::pow( sum( pow(abs(x),p) ), 1./p );
}

template<class ArrayA, class ArrayB>
double nrmsd(const ArrayA& x, const ArrayB& y)
{
  return norm(x-y) / norm(x);
}


/*! @} */

}

#endif


