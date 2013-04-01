#ifndef NUMCPP_TRIGONOMETRIC_H
#define NUMCPP_TRIGONOMETRIC_H

#include "../core.h"

namespace numcpp
{

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

}

#endif
