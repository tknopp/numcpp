#ifndef NUMCPP_HYPERBOLIC_H
#define NUMCPP_HYPERBOLIC_H

namespace numcpp
{

using std::sinh;
using std::cosh;
using std::tanh;

VECTORIZE(sinh, sinh)
VECTORIZE(cosh, cosh)
VECTORIZE(tanh, tanh)

}

#endif
