#ifndef NUMCPP_DIAG_H
#define NUMCPP_DIAG_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup diag
@brief Diagonal matrix functions
@ingroup base
@{
*/

// diagonal matrices

/*!
Create a diagonal matrix from a vector
*/
template<class T, class R>
Matrix<T> diag(const AbstractVector<T,R>& x)
{
  Matrix<T> y = zeros(x.size(),x.size());
  for(size_t i=0; i<x.size(); i++)
    y(i,i) = x(i);
  return y;
}

/*!
Create a vector containing the main diagonal of a matrix
*/
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
