#include "fftw.h"

#ifndef NUMCPP_EXTERN_BUILD
#define NUMCPP_EXTERN_BUILD extern
#endif

namespace numcpp
{

NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 >& fft_(Array< std::complex<float>, 1 >& x, int dir);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 >& fft_(Array< std::complex<double>, 1 >& x, int dir);

NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 >& ifft_(Array< std::complex<float>, 1 >& x, int dir);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 >& ifft_(Array< std::complex<double>, 1 >& x, int dir);

}
