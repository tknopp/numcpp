#ifndef ARRAY_FUNCTIONS
#define ARRAY_FUNCTIONS

#include "../core.h"
#include "initializers.h"
#include "special.h"
#include <unistd.h>

namespace numcpp
{


VECTORIZE(std::abs, abs)
VECTORIZE(std::exp, exp)
VECTORIZE(std::sin, sin)
VECTORIZE(std::cos, cos)
VECTORIZE(std::tan, tan)
VECTORIZE(std::asin, asin)
VECTORIZE(std::acos, acos)
VECTORIZE(std::atan, atan)
VECTORIZE(std::log, log)
VECTORIZE(std::sinh, sinh)
VECTORIZE(std::cosh, cosh)
VECTORIZE(std::tanh, tanh)
VECTORIZE(std::sqrt, sqrt)
VECTORIZE(std::conj, conj)
VECTORIZE(std::real, real)
VECTORIZE(std::imag, imag)
VECTORIZE(sign, sign)
VECTORIZE_ONE_ARG(std::pow,pow,int)
VECTORIZE_ONE_ARG(std::pow,pow,double)

template<class T, int D, class R>
Array<T,D> copy(const AbstractArray<T,D,R>& x)
{
  Array<T,D> y(x.shape());
  y = x;
  return y;
}

template<class T, int D>
Array<T,D> transpose_(Array<T,D>& x)
{
  std::reverse(x.strides().begin(), x.strides().end());
  return x;
}

template<class T, int D, class R>
Array<T,D> transpose(const AbstractArray<T,D,R>& x)
{
  auto y = copy(x);
  return transpose_(y);
}

/**
 * Compute the maximum entry of the AbstractArray \a x
 *
 * @ingroup ArrayFunctions
 *
 */
template<class T, int D, class R>
T max(const AbstractArray<T,D,R>& x)
{
  T y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] > y ? x[i] : y;
  return y;
}

template<class T, int D, class R>
T min(const AbstractArray<T,D,R>& x)
{
  T y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] < y ? x[i] : y;
  return y;
}

template<class T, int D, class R>
T sum(const AbstractArray<T,D,R>& x)
{
  T y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y += x[i];
  return y;
}

template<class T, int D, class R>
double mean(const AbstractArray<T,D,R>& x)
{
  double y = sum(x);
  return y / x.size();
}

template<class T, int D, class R>
double norm(const AbstractArray<T,D,R>& x, double p=2.0)
{
  return std::pow( sum( pow(abs(x),p) ), 1./p );
}


template<class T, int D, class R>
std::complex<double> mean(const AbstractArray<std::complex<T>,D,R>& x)
{
  std::complex<double> y = sum(x);
  return y / x.size();
}

template<class T, int D, class Int>
Array<T, D-1> sum(const Array<T,D>& x, Int axis) // TODO AbstractArray
{
  static_assert(D > 1, "sum(x,axis) is only supported for array of dimension D>1!");

  std::array<size_t,D-1> shape;

  copyShapeToSubArray<D>(x.shape(), shape, axis);

  Array<T, D-1> y = zeros<D-1>(shape);

  std::array<size_t,D> index;
  Iterator<D-1> it(shape);

  for(size_t k=0; k<y.size(); k++, it++)
  {
    copyShapeFromSubArray<D>(*it, index, axis);

    for(size_t i=0; i<x.shape(axis); i++)
    {
      index[axis] = i;
      y[*it] += x[index];
    }
  }

  return y;
}

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

// diff

template<class T, class R>
Vector<T> diff(const AbstractVector<T,R>& x, int order=1)
{
  Vector<T> y = x(slice(1,end)) - x(slice(0,end-1));
  if(order == 1)
    return y;
  else
    return diff(y, order -1);
}

template<class T, class R>
Matrix<T> diff(const AbstractMatrix<T,R>& x, int order=1, int axis=1)
{
  if(axis == 1)
  {
    Matrix<T> y = x(all, slice(1,end)) - x(all, slice(0,end-1));
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  } else
  {
    Matrix<T> y = x(slice(1,end), all) - x(slice(0,end-1), all);
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  }
}

template<class T, class R>
Matrix<T> rotl90(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(N-n-1,m) = A(m,n);

    return B;
}

template<class T, class R>
Matrix<T> rotr90(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(n,M-m-1) = A(m,n);

    return B;
}

template<class T, class R>
Matrix<T> rot180(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(M-m-1,N-n-1) = A(m,n);

    return B;
}

template<class T, class R>
AbstractVector<T,R>& reverse_(AbstractVector<T,R>& x)
{
    size_t N = x.shape(0);
    size_t j = N-1;
    T tmp;
    for(size_t n=0; n<floor(N/2.0); n++,j--)
    {
      tmp = x(j);
      x(j) = x(n);
      x(n) = tmp;
    }
    return x;
}

template<class T, class R>
Vector<T> reverse(const AbstractVector<T,R>& x)
{
  Vector<T> y = copy(x);
  reverse_(y);
  return y;
}




}

#endif
