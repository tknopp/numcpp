#ifndef NUMCPP_SUMS_H
#define NUMCPP_SUMS_H

#include "../core.h"


namespace numcpp
{

/*!
@file

@addtogroup sums
@brief Summation array functions
@ingroup base
@{
*/

// sum

/*!
Sum over all elements of \a x.
*/
template<class T, class U>
T sum(const AbstractArrayExpression<T, U>& x)
{
  auto y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y += x[i];
  return y;
}

/*!
Sum of \a x along a specific axis \a axis.

Reduces the dimensionality of the array by one.
*/
template<class T, class Int>
Array<T> sum(const Array<T>& x, Int axis) // TODO AbstractArray
{
  //  static_assert(D > 1, "sum(x,axis) is only supported for array of dimension D>1!");

  std::vector<size_t> shape_(x.ndims()-1);

  copyShapeToSubArray(x.shape(), shape_, axis);

  Array<T> y = zeros(shape_);

  std::vector<size_t> index(x.ndims());
  Iterator it(shape_);

  for(size_t k=0; k<y.size(); k++, it++)
  {
    copyShapeFromSubArray(*it, index, axis);

    for(size_t i=0; i<shape(x,axis); i++)
    {
      index[axis] = i;
      y[*it] += x[index];
    }
  }

  return y;
}

/*template<class T>
Array<T> sum(const Array<T>& x, int axis) // TODO AbstractArray
{
  int otherAxis = (axis == 0) ? 1 : 0;

  Array<T> y = zeros(otherAxis);

  std::array<size_t,2> index = {0, 0};

  for(; index[1] < x.shape(1); (index[1])++)
    for(; index[0] < x.shape(0); (index[0])++)
      y(index[otherAxis]) += x(index);

  return y;
}*/




// diff

/*!
Calculate the difference between elements of \a x.

The order can be specified by the parameter \a order.
*/
template<class T>
Array<T> diff(const Array<T>& x, int order=1)
{
  Array<T> y = x(slice(1,-1)) - x(slice(0,x.size()-1));
  if(order == 1)
    return y;
  else
    return diff(y, order -1);
}

/*!
Calculate the difference between elements of \a x along axis \a axis

The order can be specified by the parameter \a order.
*/
template<class T>
Array<T> diff(const Array<T>& x, int order=1, int axis=1)
{
  if(axis == 1)
  {
    Array<T> y = x(full, slice(1,-1)) - x(full, slice(0,x.shape(1)-1));
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  } else
  {
    Array<T> y = x(slice(1,-1), full) - x(slice(0,x.shape(0)-1), full);
    if(order == 1)
      return y;
    else
      return diff(y, order -1, axis);
  }
}

// cumsum

/*!
Calculate the cumulative sum over \a x.
*/
template<class T>
Array<T> cumsum(const Array<T>& x)
{
  Array<T> y(x.size());
  y[0] = x[0];

  for(size_t i=1; i<x.size(); i++)
    y[i] = x[i] + y[i-1];
  return y;
}

/*! @} */

}




#endif
