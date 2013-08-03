#ifndef NUMCPP_CORE_FUNC_H
#define NUMCPP_CORE_FUNC_H

#include "defines.h"
#include "abstractexpression.h"

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

/*!
Number of dimensions (rank) of the array \a x.
*/
int ndims(const DynTypeArray& x);

/*!
Number of elements (shape) of the array \a x along axis \a d.
*/
size_t shape(const DynTypeArray& x, int d);

/*!
Total number of elements of the array \a x.
*/
int size(const DynTypeArray& x);

/*!
Create a similar array to \a x that has the same type, dimension and shape.
*/
DynTypeArray similar(const DynTypeArray& x);

template<class Array>
Array similar(const Array& x)
{
  return Array(x.shape());
}

/*!
Create a deep copy of the array \a x.
*/
template<class Array>
Array copy(const Array& x)
{
  auto y = similar(x);
  y = x;
  return y;
}

/*!
Evaluate the array expression \a x into a new array. If x is already a dense array, no
copy will will be made but the same array will be returned (as a shallow copy).
*/
template<class T, class R>
Array<T> eval(const AbstractArrayExpression<T,R>& x)
{
  return Array<T>(x);
}

template<class T>
const Array<T>& eval(const Array<T>& x)
{
  return x;
}

/*!
Check whether the array x has the dimension dim.
*/
void checkDim(const DynTypeArray& x, int dim);

/*! @} */

}




#endif
