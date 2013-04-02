#ifndef NUMCPP_LINALG_H
#define NUMCPP_LINALG_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup linalg
@brief Linear algebra functions
@ingroup base
@{
*/

// diagonal matrices

template<class T, class R>
Matrix<T> diag(const AbstractVector<T,R>& x)
{
  Matrix<T> y = zeros(x.size(),x.size());
  for(size_t i=0; i<x.size(); i++)
    y(i,i) = x(i);
  return y;
}

template<class T, class R>
Vector<T> diag(const AbstractMatrix<T,R>& x)
{
  size_t N = x.shape(0);
  Vector<T> y = zeros(N);
  for(size_t i=0; i<N; i++)
    y(i) = x(i,i);
  return y;
}



/*! @} */

}




#endif
