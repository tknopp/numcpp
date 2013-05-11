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
template<class T, int D, class R>
double norm(const AbstractArray<T,D,R>& x, double p=2.0)
{
  if(p == 0)
    return ( sum( abs(x) != 0 ) );
  if(std::isinf(p))
    return ( max( abs(x)) );

  return std::pow( sum( pow(abs(x),p) ), 1./p );
}

template<class T, int D, class R1, class R2>
double nrmsd(const AbstractArray<T,D,R1>& x, const AbstractArray<T,D,R2>& y)
{
  return norm(x-y) / norm(x);
}


/*! @} */

}

#endif


