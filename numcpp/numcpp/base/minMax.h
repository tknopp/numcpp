#ifndef NUMCPP_MINMAX_H
#define NUMCPP_MINMAX_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup minMax
@brief Minimum and maximum array functions
@ingroup base
@{
*/


/*!
Compute the maximum entry of \a x

*/
template<class Array>
typename Array::value_type max(const Array& x)
{
  auto y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] > y ? x[i] : y;
  return y;
}

/*!
Compute the minimum entry of \a x

*/
template<class Array>
typename Array::value_type min(const Array& x)
{
  auto y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] < y ? x[i] : y;
  return y;
}

/*! @} */

}




#endif
