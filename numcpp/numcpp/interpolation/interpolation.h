#ifndef NUMCPP_INTERPOLATION_H
#define NUMCPP_INTERPOLATION_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup interpolation
@{
*/

template<class T, class U>
Array<T> interpLinear1D(const Array<U>& x, const Array<T>& y, const Array<U>& xp);



/*! @} */


}

#endif
