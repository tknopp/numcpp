#ifndef NUMCPP_TRIGONOMETRIC_H
#define NUMCPP_TRIGONOMETRIC_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup trigonometric
@brief Trigonometric array functions
@ingroup base
@{
*/

using std::cos;
using std::sin;
using std::tan;
using std::asin;
using std::acos;
using std::atan;

/*!
\def template<class T , int D, class R > ExpressionTemplate sin(const AbstractArray<T,D,R>& x)
Trigonometric sinus of \a x, elementwise.
*/

VECTORIZE(sin, sin)
VECTORIZE(cos, cos)
VECTORIZE(tan, tan)
VECTORIZE(asin, asin)
VECTORIZE(acos, acos)
VECTORIZE(atan, atan)


/*! @} */

}

#endif
