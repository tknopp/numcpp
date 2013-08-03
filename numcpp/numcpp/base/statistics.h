#ifndef NUMCPP_STATISTICS_H
#define NUMCPP_STATISTICS_H

#include "../core.h"
#include "initializers.h"

namespace numcpp
{

/*!
@file

@addtogroup statistics
@brief Statistical array functions
@ingroup base
@{
*/

/*!
Return the mean value of the array \a x.
*/
template<class Array>
DOUBLE_TYPE(typename Array::value_type) mean(const Array& x)
{
  DOUBLE_TYPE(typename Array::value_type) y = sum(x);
  return y / x.size();
}

/*!
Return the standard deviation of \a x.
*/
template<class Array>
DOUBLE_TYPE(typename Array::value_type) stdDev(const Array& x)
{
  return sqrt( mean( pow( abs( x - mean(x) ), 2) ) );
}

/*! @} */

}

#endif
