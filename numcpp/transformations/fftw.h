#ifndef FFTW_H
#define FFTW_H

#include <fftw3.h>
//#include "../core.h"

namespace numcpp
{

template<class T, int D, class R>
  Array< std::complex< typename complexBaseType<T>::type >, D >
  _fft (const AbstractArray< T, D, R >& x, int dir)
{
  auto N = x.size();

  std::array<int,D> shape;

  std::copy(std::begin(x.shape()), std::end(x.shape()), std::begin(shape));

  Array< std::complex< typename complexBaseType<T>::type >, D > y( x.shape() );

  fftw_complex *out = (fftw_complex*) y.data();
  fftw_complex *in = out;
  fftw_plan p;

  for(size_t i=0; i<N; i++)
    y[i] = x[i];

  p = fftw_plan_dft(D, shape.data(), in, out, dir, FFTW_ESTIMATE);

  if(dir == FFTW_BACKWARD)
    y *= 1. / y.size();

  fftw_execute(p);

  fftw_destroy_plan(p);

  return y;
}

template<class T, int D, class R>
  Array< std::complex< typename complexBaseType<T>::type >, D >
  fft (const AbstractArray< T, D, R >& x)
{
  return _fft(x, FFTW_FORWARD);
}

template<class T, int D, class R>
  Array< std::complex< typename complexBaseType<T>::type >, D >
  ifft (const AbstractArray< T, D, R >& x)
{
  return _fft(x, FFTW_BACKWARD);
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

template<class T>
Vector<T> fftshift(const Vector<T>& x)
{
  size_t N = x.size();
  Vector<T> y(N);
  auto k = slice(0,N/2);
  auto l = slice(N/2,N);
  y(l) = x(k);
  y(k) = x(l);
  return y;
}

template<class T>
Matrix<T> fftshift(const Matrix<T>& x)
{
  size_t N = x.shape(0);
  size_t M = x.shape(1);
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

}

#endif // FFTW_H
