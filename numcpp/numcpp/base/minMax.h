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
/*template<class T, int D, class R>
T max(const AbstractArray<T,D,R>& x)
{
  T y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] > y ? x[i] : y;
  return y;
}*/

template<class Array>
auto max(const Array& x) -> decltype(x[0])
{
  decltype(x[0]) y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] > y ? x[i] : y;
  return y;
}

/*!
Compute the minimum entry of \a x

*/
template<class T, int D, class R>
T min(const AbstractArray<T,D,R>& x)
{
  T y = x[0];
  for(size_t i=1; i<x.size(); i++)
    y = x[i] < y ? x[i] : y;
  return y;
}

/*! @} */

}




#endif
