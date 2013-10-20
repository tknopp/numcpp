#ifndef NUMCPP_CORE_FUNC_H
#define NUMCPP_CORE_FUNC_H

#include "defines.h"
#include "abstractexpression.h"
#include "dyntypearray.h"

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
size_t size(const DynTypeArray& x);

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

/*! 
Ostream operater for Array
*/
template<class T>
std::ostream& operator<< (std::ostream& os, const Array<T>& x)
{
  os << "Array<"<< typeStr(x.elemType())
     << "," << x.ndims() << "> {\n";
  os << "  shape = ";
  for(int i=0; i<x.ndims(); i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<x.ndims(); i++)
    os << x.strides(i) << " ";
  os << "\n";
  os << "  offset = " << x.offset() << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(x.ndims()-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l < x.shape(x.ndims()-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
  }
  os << "}\n";
  return os;
}


/*! @} */

}




#endif
