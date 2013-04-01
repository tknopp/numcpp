#ifndef NUMCPP_ARRAY_FUNCTIONS
#define NUMCPP_ARRAY_FUNCTIONS

#include "../core.h"
#include "initializers.h"
#include "special.h"
#include <unistd.h>

namespace numcpp
{

template<class T, int D, class R>
Array<T,D> copy(const AbstractArray<T,D,R>& x)
{
  return Array<T,D>(x);
}

template<class T, int D, class R>
Array<T,D> eval(const AbstractArray<T,D,R>& x)
{
  return Array<T,D>(x);
}

template<class T, int D>
Array<T,D> eval(const Array<T,D>& x)
{
  return x;
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
    Matrix<T> y = x(full, slice(1,end)) - x(full, slice(0,end-1));
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  } else
  {
    Matrix<T> y = x(slice(1,end), full) - x(slice(0,end-1), full);
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  }
}



}

#endif
