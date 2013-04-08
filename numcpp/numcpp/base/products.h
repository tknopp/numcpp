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
  {
    T tmp = 0;
    for(size_t n=0; n<N; n++)
      tmp += A(m,n) * x(n);
    y(m) = tmp;
  }
  return y;
}

template<class T, class U, class R, class V>
  Matrix< COMMON_TYPE(T,U) >
  dot(const AbstractMatrix<T,R>& A, const AbstractMatrix<U,V>& B)
{
  auto M = A.shape(0);
  auto N = A.shape(1);
  auto K = B.shape(1);
  Matrix< COMMON_TYPE(T,U) > C = zeros(M,K);

  omp_lock_t* lock = new omp_lock_t;
  omp_init_lock (lock);
  Vector<U> tmpV(N);

  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for(size_t k=0; k<K; k++)
  {
    omp_set_lock (lock);
     tmpV = copy(B(full,k));
      omp_unset_lock (lock);
    for(size_t m=0; m<M; m++)
    {
      T tmp = 0;
      for(size_t n=0; n<N; n++)
        tmp += A(m,n) * tmpV(n);
      C(m,k) = tmp;
    }
  }
    omp_destroy_lock (lock);
  return C;
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
