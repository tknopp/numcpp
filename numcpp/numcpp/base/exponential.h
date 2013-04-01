#ifndef NUMCPP_EXPONENTIAL_H
#define NUMCPP_EXPONENTIAL_H

#include "../core.h"

namespace numcpp
{

using std::exp;
using std::log;
using std::log10;
using std::log2;

VECTORIZE(exp, exp)
VECTORIZE(log, log)
VECTORIZE(log10, log10)
VECTORIZE(log2, log2)

}

#endif
