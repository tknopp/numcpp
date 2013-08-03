#ifndef NUMCPP_EXPRESSIONS_H
#define NUMCPP_EXPRESSIONS_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup expressions
@brief vectorized array functions
@ingroup base
@{
*/

using std::abs;
using std::sqrt;
using std::pow;
using std::round;

VECTORIZE(abs, abs)
VECTORIZE(sqrt, sqrt)
VECTORIZE_ONE_ARG(pow,pow,int)
VECTORIZE_ONE_ARG(pow,pow,double)

using std::sinh;
using std::cosh;
using std::tanh;

VECTORIZE(sinh, sinh)
VECTORIZE(cosh, cosh)
VECTORIZE(tanh, tanh)

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

/*!
@fn template <class T, class Op> Array_cos < AbstractArray<T,Op::dim, Op> > cos( const AbstractArray<T,Op::dim, Op>& op)
@brief Trigonometric cosine of \a x, elementwise.
*/

using std::exp;
using std::log;
using std::log10;
using std::log2;

VECTORIZE(exp, exp)
VECTORIZE(log, log)
VECTORIZE(log10, log10)
VECTORIZE(log2, log2)

using std::round;
using std::ceil;
using std::floor;
using std::trunc;

VECTORIZE(round, round)
VECTORIZE(ceil, ceil)
VECTORIZE(floor, floor)
VECTORIZE(trunc, trunc)

/*! @} */

}

#endif
