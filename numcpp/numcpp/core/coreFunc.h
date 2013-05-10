#ifndef NUMCPP_CORE_FUNC_H
#define NUMCPP_CORE_FUNC_H

#include "array.h"

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

using std::abs;
using std::sqrt;
using std::pow;
using std::round;

VECTORIZE(abs, abs)
VECTORIZE(sqrt, sqrt)
VECTORIZE_ONE_ARG(pow,pow,int)
VECTORIZE_ONE_ARG(pow,pow,double)

/*!
Number of dimensions (rank) of the array \a x.
*/
template <class T, int D, class Derived>
int ndims(const AbstractArray<T,D,Derived>& x)
{
  return D;
}

/*!
Number of elements (shape) of the array \a x along axis \a d.
*/
template <class T, int D, class Derived>
size_t shape(const AbstractArray<T,D,Derived>& x, int d)
{
  return x.shape()[d];
}

/*!
Total number of elements of the array \a x.
*/
template <class T, int D, class Derived>
int size(const AbstractArray<T,D,Derived>& x)
{
  return x.size();
}

/*!
Create a similar array to \a x that has the same type, dimension and shape.
*/
template<class T, int D, class R>
Array<T,D> similar(const AbstractArray<T,D,R>& x)
{
  return Array<T,D>(x.shape());
}

/*!
Create a deep copy of the array \a x.
*/
template<class T, int D, class R>
Array<T,D> copy(const AbstractArray<T,D,R>& x)
{
  auto y = similar(x);
  y = x;
  return y; //Array<T,D>(x);
}

/*!
Evaluate the array expression \a x into a new array. If x is already a dense array, no
copy will will be made but the same array will be returned (as a shallow copy).
*/
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


/*! @} */

}




#endif
