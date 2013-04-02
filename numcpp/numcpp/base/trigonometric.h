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

VECTORIZE(sin, sin)
VECTORIZE(cos, cos)
VECTORIZE(tan, tan)
VECTORIZE(asin, asin)
VECTORIZE(acos, acos)
VECTORIZE(atan, atan)

/*!
@fn template <class T, class Op> Array_sin < AbstractArray<T,Op::dim, Op> > sin( const AbstractArray<T,Op::dim, Op>& op)
@brief Trigonometric sinus of \a x, elementwise.
*/


/*! @} */

}

#endif
