#ifndef NUMCPP_COREFUNC_H
#define NUMCPP_COREFUNC_H

#include "array.h"

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/


VECTORIZE(std::abs, abs)
VECTORIZE(std::sqrt, sqrt)
VECTORIZE_ONE_ARG(std::pow,pow,int)
VECTORIZE_ONE_ARG(std::pow,pow,double)

/*!
Number of dimensions (i.e. rank) ao the array \a x.
*/
template <class T, int D, class Derived>
int ndims(const AbstractArray<T,D,Derived>& x)
{
  return D;
}

template <class T, int D, class Derived>
size_t shape(const AbstractArray<T,D,Derived>& x, int d)
{
  return x.shape(d);
}

/*!
Create a deep copy of the array \a x.
*/
template<class T, int D, class R>
Array<T,D> copy(const AbstractArray<T,D,R>& x)
{
  return Array<T,D>(x);
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
