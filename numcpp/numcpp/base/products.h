#ifndef NUMCPP_PRODUCTS_H
#define NUMCPP_PRODUCTS_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup products
@brief Matrix and vector products
@ingroup base
@{
*/

// dot functions

/*!
Calculate the tensor dot product between the arrays \a x and \a y.
The last dimension of \a x and the first dimension of \a y must have
the same size. If \a x and \a y are both vectors, a scalar value
is returned. If \a is a matrix and \a y is a vector, a classical
matrix vector mutiplication is carried out.
*/
template<class T, class U, int D, int L, class R, class V>
  Array<COMMON_TYPE(T,U), D+L-2>
  dot(const AbstractArray<T,D,R>& x, const AbstractArray<U,L,V>& y);

template<class T, class U, class R, class V>
  COMMON_TYPE(T,U)
  dot(const AbstractVector<T,R>& x, const AbstractVector<U,V>& y)
{
  return sum(x*y);
}


template<class T, class U, class R, class V>
  Vector< COMMON_TYPE(T,U) >
  dot(const AbstractMatrix<T,R>& A, const AbstractVector<U,V>& x)
{
  auto M = A.shape(0);
  auto N = A.shape(1);
  Vector< COMMON_TYPE(T,U) > y = zeros(M);

  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for(size_t m=0; m<M; m++)
    for(size_t n=0; n<N; n++)
      y[m] += A(m,n) * x[n];
  return y;
}

template<class T, class U, class R, class V>
  COMMON_TYPE(T,U)
  vdot(const AbstractVector<T,R>& x, const AbstractVector<U,V>& y)
{
  return sum(conj(x)*y);
}

template<class T, class U, class R, class V>
  Vector< COMMON_TYPE(T,U) >
  vdot(const AbstractMatrix<T,R>& A, const AbstractVector<U,V>& x)
{
  auto M = A.shape(0);
  auto N = A.shape(1);
  Vector< COMMON_TYPE(T,U) > y = zeros(N);

  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for(size_t m=0; m<M; m++)
    for(size_t n=0; n<N; n++)
      y[n] += A(m,n) * x[m];
  return y;
}

/*! @} */

}

#endif
