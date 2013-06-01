#ifndef NUMCPP_ROUNDING_H
#define NUMCPP_ROUNDING_H

#include "../core.h"

namespace numcpp
{

using std::round;
using std::ceil;
using std::floor;
using std::trunc;

VECTORIZE(round, round)
VECTORIZE(ceil, ceil)
VECTORIZE(floor, floor)
VECTORIZE(trunc, trunc)

}

#endif
