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

template<class T, int D, class R>
double norm(const AbstractArray<T,D,R>& x, double p=2.0)
{
  if(p == 0)
    return ( sum( abs(x) != 0 ) );
  if(std::isinf(p))
    return ( max( abs(x)) );

  return std::pow( sum( pow(abs(x),p) ), 1./p );
}


/*! @} */

}

#endif


