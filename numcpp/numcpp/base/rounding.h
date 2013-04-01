#ifndef NUMCPP_ROUNDING_H
#define NUMCPP_ROUNDING_H

#include "../core.h"

namespace numcpp
{

VECTORIZE(std::round, round)
VECTORIZE(std::ceil, ceil)
VECTORIZE(std::floor, floor)
VECTORIZE(std::trunc, trunc)

}

#endif
