#include "fft.h"
#include <fftw3.h>

namespace numcpp
{

Array< std::complex<double> >&
internal_fft_ (Array< std::complex<double> >& x, int dir)
{
  auto N = x.size();

  int shape_[x.ndims()];

  for(int d=0; d<x.ndims(); d++)
    shape_[d] = shape(x,d);
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftw_complex *out = (fftw_complex*) x.data();
  fftw_complex *in = out;
  fftw_plan p;

  p = fftw_plan_dft(x.ndims(), shape_, in, out, dir, FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}

Array< std::complex<float> >&
internal_fft_ (Array< std::complex<float> >& x, int dir)
{
  auto N = x.size();

  int shape_[x.ndims()];

  for(int d=0; d<x.ndims(); d++)
    shape_[d] = shape(x,d);
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftwf_complex *out = (fftwf_complex*) x.data();
  fftwf_complex *in = out;
  fftwf_plan p;

  p = fftwf_plan_dft(x.ndims(), shape_, in, out, dir, FFTW_ESTIMATE);

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return x;
}

template<class T>
Array< std::complex<T> >&
fft_(Array< std::complex<T> >& x, int dir)
{
  return internal_fft_(x, dir);
}

template Array<cfloat>& fft_<float>(Array<cfloat>& x, int dir);
template Array<cdouble>& fft_<double>(Array<cdouble>& x, int dir);

template<class T>
Array< std::complex<T> >&
ifft_(Array< std::complex<T> >& x, int dir)
{
  internal_fft_(x, dir);
  x *= 1. / x.size();
  return x;
}

template Array<cfloat>& ifft_<float>(Array<cfloat>& x, int dir);
template Array<cdouble>& ifft_<double>(Array<cdouble>& x, int dir);


/////////////// R2R FFTs //////////////////

Array<double>&
internal_fft_r2r_ (Array<double>& x, const int kind)
{
  auto N = x.size();

  int shape_[x.ndims()];
  int kinds[x.ndims()];

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shape(x, d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftw_plan p;

  p = fftw_plan_r2r(x.ndims(), shape_, x.data(), x.data(), (fftwf_r2r_kind*) kinds, FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}


Array<float>&
internal_fft_r2r_ (Array<float>& x, const int kind)
{
  auto N = x.size();

  int shape_[x.ndims()];
  int kinds[x.ndims()];

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shape(x,d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftwf_plan p;

  p = fftwf_plan_r2r(x.ndims(), shape_, x.data(), x.data(), (fftwf_r2r_kind*) kinds, FFTW_ESTIMATE);

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return x;
}

template<class T>
Array<T>&
dct_(Array<T>& x)
{
  return internal_fft_r2r_(x, FFTW_REDFT10);
}

template Array<float>& dct_<float>(Array<float>& x);
template Array<double>& dct_<double>(Array<double>& x);

template<class T>
Array<T>&
idct_(Array<T>& x)
{
  internal_fft_r2r_(x, FFTW_REDFT01);
  x *= 1. / (2 * x.size());
  return x;
}

template Array<float>& idct_<float>(Array<float>& x);
template Array<double>& idct_<double>(Array<double>& x);

}
