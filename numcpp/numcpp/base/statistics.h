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

template<class T, int D, class R>
double mean(const AbstractArray<T,D,R>& x)
{
  double y = sum(x);
  return y / x.size();
}


template<class T, int D, class R>
std::complex<double> mean(const AbstractArray<std::complex<T>,D,R>& x)
{
  std::complex<double> y = sum(x);
  return y / x.size();
}

template<class T, int D, class R>
double stdDev(const AbstractArray<T,D,R>& x)
{
  return sqrt( mean( pow( abs( x - mean(x) ), 2) ) );
}

/*! @} */

}

#endif
