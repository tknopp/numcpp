#include "fftw.h"

#ifndef NUMCPP_EXTERN_BUILD
#define NUMCPP_EXTERN_BUILD extern
#endif

namespace numcpp
{

NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 >& fft_(Array< std::complex<float>, 1 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 >& fft_(Array< std::complex<float>, 2 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 >& fft_(Array< std::complex<float>, 3 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 >& fft_(Array< std::complex<double>, 1 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 >& fft_(Array< std::complex<double>, 2 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 >& fft_(Array< std::complex<double>, 3 >& x);

NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 >& ifft_(Array< std::complex<float>, 1 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 >& ifft_(Array< std::complex<float>, 2 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 >& ifft_(Array< std::complex<float>, 3 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 >& ifft_(Array< std::complex<double>, 1 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 >& ifft_(Array< std::complex<double>, 2 >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 >& ifft_(Array< std::complex<double>, 3 >& x);


/*NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 > fft(const AbstractArray< float, 1, Array<float, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 > fft(const AbstractArray< float, 2, Array<float, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 > fft(const AbstractArray< float, 3, Array<float, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 > fft(const AbstractArray< std::complex<float>, 1, Array<std::complex<float>, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 > fft(const AbstractArray< std::complex<float>, 2, Array<std::complex<float>, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 > fft(const AbstractArray< std::complex<float>, 3, Array<std::complex<float>, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 > fft(const AbstractArray< double, 1, Array<double, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 > fft(const AbstractArray< double, 2, Array<double, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 > fft(const AbstractArray< double, 3, Array<double, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 > fft(const AbstractArray< std::complex<double>, 1, Array<std::complex<double>, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 > fft(const AbstractArray< std::complex<double>, 2, Array<std::complex<double>, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 > fft(const AbstractArray< std::complex<double>, 3, Array<std::complex<double>, 3> >& x);

NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 > ifft(const AbstractArray< float, 1, Array<float, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 > ifft(const AbstractArray< float, 2, Array<float, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 > ifft(const AbstractArray< float, 3, Array<float, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 1 > ifft(const AbstractArray< std::complex<float>, 1, Array<std::complex<float>, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 2 > ifft(const AbstractArray< std::complex<float>, 2, Array<std::complex<float>, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<float>, 3 > ifft(const AbstractArray< std::complex<float>, 3, Array<std::complex<float>, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 > ifft(const AbstractArray< double, 1, Array<double, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 > ifft(const AbstractArray< double, 2, Array<double, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 > ifft(const AbstractArray< double, 3, Array<double, 3> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 1 > ifft(const AbstractArray< std::complex<double>, 1, Array<std::complex<double>, 1> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 2 > ifft(const AbstractArray< std::complex<double>, 2, Array<std::complex<double>, 2> >& x);
NUMCPP_EXTERN_BUILD template Array< std::complex<double>, 3 > ifft(const AbstractArray< std::complex<double>, 3, Array<std::complex<double>, 3> >& x);
*/

}
