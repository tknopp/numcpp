#ifndef NUMCPP_PRODUCTS_H
#define NUMCPP_PRODUCTS_H

#include "../core.h"

namespace numcpp
{

// dot functions

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
  Vector< COMMON_TYPE(T,U) > y = zeros(M);

  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for(size_t m=0; m<M; m++)
    for(size_t n=0; n<N; n++)
      y[m] += conj(A(m,n)) * x[n];
  return y;
}

}

#endif
