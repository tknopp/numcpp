#ifndef NUMCPP_FILTER_H
#define NUMCPP_FILTER_H

#include "../core.h"
#include "initializers.h"

namespace numcpp
{


template<class T, class R>
AbstractArray<T,R>& medfilt_(AbstractArray<T,R>& x, size_t kernelSize=3)
{
    size_t N = x.shape(0);

    size_t j = N-1;
    T tmp;
    for(size_t n=0; n<floor(N/2.0); n++,j--)
    {
      tmp = x(j);
      x(j) = x(n);
      x(n) = tmp;
    }
    return x;
}

}

#endif // NUMCPP_FILTER_H
