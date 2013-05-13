#ifndef NUMCPP_FFTW_H
#define NUMCPP_FFTW_H

#include <utility>
#include <fftw3.h>
#include "../core.h"


namespace numcpp
{


/*!
@file

@addtogroup fft
@{
*/

template<int D>
Array< std::complex<double>, D >&
internal_fft_ (Array< std::complex<double>, D >& x, int dir)
{
  auto N = x.size();

  std::array<int,D> shape_;

  for(int d=0; d<D; d++)
    shape_[d] = shape(x,d);
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftw_complex *out = (fftw_complex*) x.data();
  fftw_complex *in = out;
  fftw_plan p;

  p = fftw_plan_dft(D, shape_.data(), in, out, dir, FFTW_ESTIMATE);

  if(dir == FFTW_BACKWARD)
    x *= 1. / x.size();

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}

template<int D>
Array< std::complex<float>, D >&
internal_fft_ (Array< std::complex<float>, D >& x, int dir)
{
  auto N = x.size();

  std::array<int,D> shape_;

  for(int d=0; d<D; d++)
    shape_[d] = shape(x,d);
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftwf_complex *out = (fftwf_complex*) x.data();
  fftwf_complex *in = out;
  fftwf_plan p;

  p = fftwf_plan_dft(D, shape_.data(), in, out, dir, FFTW_ESTIMATE);

  if(dir == FFTW_BACKWARD)
    x *= 1. / x.size();

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return x;
}

/*!
Calculates the FFT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::ifft_ \sa ::ifft
*/
template<class T, int D>
Array< std::complex<T>, D >&
fft_(Array< std::complex<T>, D >& x)
{
  return internal_fft_(x, FFTW_FORWARD);
}

/*!
Calculates the inverse FFT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::fft_ \sa ::ifft
*/
template<class T, int D>
Array< std::complex<T>, D >&
ifft_(Array< std::complex<T>, D >& x)
{
  return internal_fft_(x, FFTW_BACKWARD);
}

/*!
Calculates the FFT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft_ \sa ::ifft_ \sa ::ifft
*/
template<class T, int D, class R>
Array< std::complex< COMPLEX_BASE_TYPE(T) >, D >
fft(const AbstractArray< T, D, R >& x)
{
  Array< std::complex< COMPLEX_BASE_TYPE(T) >, D > y(x.shape());
  y = x;
  fft_(y);
  return y;
}

/*!
Calculates the inverse FFT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::ifft_ \sa ::fft_
*/
template<class T, int D, class R>
Array< std::complex< COMPLEX_BASE_TYPE(T) >, D >
ifft(const AbstractArray< T, D, R >& x)
{
  Array< std::complex< COMPLEX_BASE_TYPE(T)>, D > y(x.shape());
  y = x;
  ifft_(y);
  return y;
}


/////////////// R2R FFTs //////////////////

template<int D>
Array<double, D >&
internal_fft_r2r_ (Array<double, D >& x, const int kind)
{
  auto N = x.size();

  std::array<int,D> shape_;
  int kinds[D];

  for(int d=0; d<D; d++)
  {
    shape_[d] = shape(x, d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftw_plan p;

  p = fftw_plan_r2r(D, shape_.data(), x.data(), x.data(), (fftwf_r2r_kind*) kinds, FFTW_ESTIMATE);

  fftw_execute(p);

  fftw_destroy_plan(p);

  return x;
}


template<int D>
Array<float, D >&
internal_fft_r2r_ (Array<float, D >& x, const int kind)
{
  auto N = x.size();

  std::array<int,D> shape_;
  int kinds[D];

  for(int d=0; d<D; d++)
  {
    shape_[d] = shape(x,d);
    kinds[d] = kind;
  }
  //std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  fftwf_plan p;

  p = fftwf_plan_r2r(D, shape_.data(), x.data(), x.data(), (fftwf_r2r_kind*) kinds, FFTW_ESTIMATE);

  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return x;
}

/*!
Calculates the DCT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T, int D>
Array< T, D >&
dct_(Array< T, D >& x)
{
  return internal_fft_r2r_(x, FFTW_REDFT10);
}

/*!
Calculates the inverse DCT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T, int D>
Array< T, D >&
idct_(Array< T, D >& x)
{
  internal_fft_r2r_(x, FFTW_REDFT01);
  x *= 1. / (2 * x.size());
  return x;
}


/*!
Calculates the DCT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T, int D, class R>
Array< T, D >
dct(const AbstractArray< T, D, R >& x)
{
  Array< T, D > y(x.shape());
  y = x;
  dct_(y);
  return y;
}

/*!
Calculates the inverse DCT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T, int D, class R>
Array< T, D >
idct(const AbstractArray< T, D, R >& x)
{
  Array< T, D > y(x.shape());
  y = x;
  idct_(y);
  return y;
}























template<class T, size_t D>
class FFTMatrix : public AbstractMatrix<T,FFTMatrix<T,D> >
{
public:
  typedef T value_type;

  template<class...A>
  FFTMatrix(A...args)
   : fftShape_({((size_t)args)...})
  {
  }

  const std::array<size_t,D>& fftShape() const
  {
    return fftShape_;
  }

private:
  std::array<size_t, D> fftShape_;
};

template<class T, size_t D>
Vector<T> dot(const FFTMatrix<T,D>& A, const Vector<T>& x )
{
  return reshape(fft(reshape(x,A.fftShape() )), x.size());
}

template<class T, size_t D>
Vector<T> solve(const FFTMatrix<T,D>& A, const Vector<T>& x )
{
  return  reshape(ifft(reshape(x,A.fftShape() )), x.size());
}

/*!
Perform an fftshift on the array \a x.

For a 1D array, an fftshifft essentially swaps the first and the second half of the array.
In 2D the upper left and the lower right quarter as well as the upper right and lower left quarter
are swapped.
*/
template<class T, int D>
Array<T,D> fftshift(const Array<T,D>& x);

/*!
Perform an inplace fftshift on the array \a x.

For a 1D array, an fftshifft essentially swaps the first and the second half of the array.
In 2D the upper left and the lower right quarter as well as the upper right and lower left quarter
are swapped.
*/
template<class T, int D>
Array<T,D>& fftshift_(Array<T,D>& x);

template<class T>
Vector<T>& fftshift_(Vector<T>& x)
{
  size_t N = x.size();
  if(N % 2 == 0)
  {
    for(size_t n=0; n<N/2; n++)
    {
      swap(x(n),x(N/2+n));
    }
  }
  else
  {
    T tmpC = x(N/2);
    for(size_t n=0; n<N/2; n++)
    {
      x(n+N/2) = x(n);
      x(n) = x(n+N/2+1);
    }
    x(N-1) = tmpC;
  }

  return x;
}

template<class T>
Vector<T>& ifftshift_(Vector<T>& x)
{
  size_t N = x.size();
  if(N % 2 == 0)
  {
    for(size_t n=0; n<N/2; n++)
    {
      swap(x(n),x(N/2+n));
    }
  }
  else
  {
    T tmpC = x(N/2);
    for(size_t n=0; n<N/2; n++)
    {
      x(N/2-n) = x(N-1-n);
      x(N-1-n) = x(N/2-1-n);
    }
    x(0) = tmpC;
  }

  return x;
}

template<class T>
Vector<T> fftshift(const Vector<T>& x)
{
  auto y = copy(x);
  return fftshift_(y);
}

template<class T>
Vector<T> ifftshift(const Vector<T>& x)
{
  auto y = copy(x);
  return ifftshift_(y);
}


template<class T>
Matrix<T> fftshift(const Matrix<T>& x)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);
  Matrix<T> y(N,M);

  auto k = slice(0,N/2);
  auto l = slice(N/2,N);
  auto m = slice(0,M/2);
  auto n = slice(M/2,M);

  y(l,n) = x(k,m);
  y(k,m) = x(l,n);
  y(l,m) = x(k,n);
  y(k,n) = x(l,m);

  return y;
}

/*! @} */

}

#endif
