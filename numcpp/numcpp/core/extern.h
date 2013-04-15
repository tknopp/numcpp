#include "array.h"

#ifndef NUMCPP_EXTERN_BUILD
#define NUMCPP_EXTERN_BUILD extern
#endif

namespace numcpp
{

//NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 >& fft_(Array< std::complex<float>, 1 >& x);

NUMCPP_EXTERN_BUILD template class Array<int,1>;

}
