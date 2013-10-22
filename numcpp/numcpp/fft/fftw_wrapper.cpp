#include "fft.h"
#include <fftw3.h>

namespace numcpp
{

Array< std::complex<double> >&
internal_fft_ (Array< std::complex<double> >& x, int dir)
{
  auto N = x.size();

  std::vector<int> shape_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
    shape_[d] = shape(x,d);

  fftw_complex *out = (fftw_complex*) x.data();
  fftw_complex *in = out;
  fftw_plan p;

  p = fftw_plan_dft(x.ndims(), shape_.data(), in, out, dir, FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}

Array< std::complex<float> >&
internal_fft_ (Array< std::complex<float> >& x, int dir)
{
  auto N = x.size();

  std::vector<int> shape_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
    shape_[d] = shape(x,d);

  fftwf_complex *out = (fftwf_complex*) x.data();
  fftwf_complex *in = out;
  fftwf_plan p;

  p = fftwf_plan_dft(x.ndims(), shape_.data(), in, out, dir, FFTW_ESTIMATE);

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

/////////////// R2C FFTs //////////////////

template<>
Array< std::complex<double> > rfft(const Array<double>& x)
{
  std::vector<int> shape_(x.ndims());
  std::vector<size_t> shapeOut_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shapeOut_[d] = shape(x, d);
  }

  shapeOut_[x.ndims()-1] = shapeOut_[x.ndims()-1] / 2 +1;

  Array<cdouble> y(shapeOut_);

  fftw_plan p = fftw_plan_dft_r2c(x.ndims(), shape_.data(), x.data(), (fftw_complex*) y.data(), FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return y;
}

template<>
Array< std::complex<float> > rfft(const Array<float>& x)
{
  std::vector<int> shape_(x.ndims());
  std::vector<size_t> shapeOut_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shapeOut_[d] = shape(x, d);
  }

  shapeOut_[x.ndims()-1] = shapeOut_[x.ndims()-1] / 2 +1;

  Array<cfloat> y(shapeOut_);

  fftwf_plan p = fftwf_plan_dft_r2c(x.ndims(), shape_.data(), x.data(), (fftwf_complex*) y.data(), FFTW_ESTIMATE);

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return y;
}

template<>
Array<double> irfft(const Array< std::complex<double> >& x)
{
  std::vector<int> shape_(x.ndims());
  std::vector<size_t> shapeOut_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shapeOut_[d] = shape(x, d);
  }

  shapeOut_[x.ndims()-1] = (shapeOut_[x.ndims()-1] - 1) *2;
  shape_[x.ndims()-1] = shapeOut_[x.ndims()-1];

  Array<double> y(shapeOut_);

  fftw_plan p = fftw_plan_dft_c2r(x.ndims(), shape_.data(), (fftw_complex*) x.data(), y.data(), FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  y /= y.size();

  return y;
}

template<>
Array<float> irfft(const Array< std::complex<float> >& x)
{
  std::vector<int> shape_(x.ndims());
  std::vector<size_t> shapeOut_(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shapeOut_[d] = shape(x, d);
  }

  shapeOut_[x.ndims()-1] = (shapeOut_[x.ndims()-1] - 1) *2;
  shape_[x.ndims()-1] = shapeOut_[x.ndims()-1];

  Array<float> y(shapeOut_);

  fftwf_plan p = fftwf_plan_dft_c2r(x.ndims(), shape_.data(), (fftwf_complex*) x.data(), y.data(), FFTW_ESTIMATE);

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  y /= y.size();

  return y;
}

/////////////// R2R FFTs //////////////////

Array<double>&
internal_fft_r2r_ (Array<double>& x, const int kind)
{
  auto N = x.size();

  std::vector<int> shape_(x.ndims());
  std::vector<int> kinds(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shape(x, d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftw_plan p;

  p = fftw_plan_r2r(x.ndims(), shape_.data(), x.data(), x.data(), (fftwf_r2r_kind*) kinds.data(), FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}


Array<float>&
internal_fft_r2r_ (Array<float>& x, const int kind)
{
  auto N = x.size();

  std::vector<int> shape_(x.ndims());
  std::vector<int> kinds(x.ndims());

  for(int d=0; d<x.ndims(); d++)
  {
    shape_[d] = shape(x,d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftwf_plan p;

  p = fftwf_plan_r2r(x.ndims(), shape_.data(), x.data(), x.data(), (fftwf_r2r_kind*) kinds.data(), FFTW_ESTIMATE);

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
