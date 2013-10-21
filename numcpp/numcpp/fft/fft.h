#ifndef NUMCPP_FFT_H
#define NUMCPP_FFT_H

#include <utility>
#include "../core.h"

namespace numcpp
{


/*!
@file

@addtogroup fft
@{
*/

/*!
Calculates the FFT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::ifft_ \sa ::ifft
*/
template<class T>
Array< std::complex<T> >&
fft_(Array< std::complex<T> >& x, int dir=-1);

/*!
Calculates the inverse FFT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::fft_ \sa ::ifft
*/
template<class T>
Array< std::complex<T> >&
ifft_(Array< std::complex<T> >& x, int dir=1);

/*!
Calculates the FFT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft_ \sa ::ifft_ \sa ::ifft
*/
template<class T>
Array< std::complex< COMPLEX_BASE_TYPE(T) > >
fft(const Array<T>& x)
{
  Array< std::complex< COMPLEX_BASE_TYPE(T) > > y(x.shape());
  y = x;
  fft_(y);
  return y;
}

/*!
Calculates the inverse FFT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional fft is performed
\sa ::fft \sa ::ifft_ \sa ::fft_
*/
template<class T>
Array< std::complex< COMPLEX_BASE_TYPE(T) > >
ifft(const Array<T>& x)
{
  Array< std::complex< COMPLEX_BASE_TYPE(T)> > y(x.shape());
  y = x;
  ifft_(y);
  return y;
}

template<class T>
Array< std::complex<T> > rfft(const Array<T>& x);

template<class T>
Array<T> irfft(const Array< std::complex<T> >& x);

/*!
Calculates the DCT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T>
Array<T>&
dct_(Array<T>& x);

/*!
Calculates the inverse DCT of the array \a x (inplace).

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T>
Array<T>&
idct_(Array<T>& x);

/*!
Calculates the DCT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T>
Array<T>
dct(const Array<T>& x)
{
  Array<T> y(x.shape());
  y = x;
  dct_(y);
  return y;
}

/*!
Calculates the inverse DCT of the array \a x.

Note that for multidimensional input arrays, the respective multidimensional DCT is performed
*/
template<class T>
Array<T>
idct(const Array<T>& x)
{
  Array<T> y(x.shape());
  y = x;
  idct_(y);
  return y;
}











template<class T>
class FFTArray
{
public:
  template<class...A>
  FFTArray(A...args)
   : fftShape_({((size_t)args)...})
  {
  }

  const std::vector<size_t>& fftShape() const
  {
    return fftShape_;
  }

private:
  std::vector<size_t> fftShape_;
};

template<class T>
Array<T> dot(const FFTArray<T>& A, const Array<T>& x )
{
  return reshape(fft(reshape(x, A.fftShape())), x.size());
}

template<class T>
Array<T> solve(const FFTArray<T>& A, const Array<T>& x )
{
  return  reshape(ifft(reshape(x,A.fftShape() )), x.size());
}


template<class T>
class SparseFFTArray
{
public:

  template<class...A>
  SparseFFTArray(Array<size_t> indices_, A...args)
   : indices(indices_)
   , fftShape_({((size_t)args)...})
   , shape_(2)
  {
    shape_[0] = indices.size();
    shape_[1] = prod(fftShape_);
  }

  const std::vector<size_t>& fftShape() const
  {
    return fftShape_;
  }

  const std::vector<size_t>& shape() const
  {
    return shape_;
  }

  Array<size_t> indices;
private:
  std::vector<size_t> fftShape_, shape_;
};

template<class T>
Array<T> dot(SparseFFTArray<T>& A, const Array<T>& x )
{
  auto y = copy(x);
  auto tmp = reshape(y,A.fftShape());
  fft_(tmp);
  Array<T> z(A.indices.size());
  for(size_t i=0; i<A.indices.size(); i++)
    z(i) = y(A.indices(i))  / sqrt(prod(A.fftShape())) ;
  return z;
}

template<class T>
Array<T> solve(SparseFFTArray<T>& A, const Array<T>& x )
{
  Array<T> y = zeros(A.shape()[1]);
  for(size_t i=0; i<A.indices.size(); i++)
    y(A.indices(i)) = x(i);
  auto tmp = reshape(y, A.fftShape());
  ifft_(tmp);
  y *= sqrt(prod(A.fftShape()));

  return  y;
}


/*!
Perform an fftshift on the array \a x.

For a 1D array, an fftshifft essentially swaps the first and the second half of the array.
In 2D the upper left and the lower right quarter as well as the upper right and lower left quarter
are swapped.
*/
template<class T>
Array<T> fftshift(const Array<T>& x);

/*!
Perform an inplace fftshift on the array \a x.

For a 1D array, an fftshifft essentially swaps the first and the second half of the array.
In 2D the upper left and the lower right quarter as well as the upper right and lower left quarter
are swapped.
*/

/*
template<class T>
Array<T>& ifftshift_(Array<T>& x)
{
  size_t N = x.size();
  if(N % 2 == 0)
  {
    for(size_t n=0; n<N/2; n++)
    {
      std::swap(x(n),x(N/2+n));
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
Array<T> fftshift(const Array<T>& x)
{
  auto y = copy(x);
  return fftshift_(y);
}*/

template<class T>
Array<T> ifftshift(const Array<T>& x)
{
  auto y = copy(x);
  return ifftshift_(y);
}


/*template<class T>
Array<T> fftshift(const Array<T>& x)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);
  Array<T> y(N,M);

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

template<class T>
Array<T>& fftshift_(Array<T>& x)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);

  for(size_t n=0; n<N; n++)
  {
    auto sl = x(n,full);
    fftshift_(sl);
  }

  for(size_t m=0; m<M; m++)
  {
    auto sl = x(full,m);
    fftshift_(sl);
  }

  return x;
}

template<class T>
Array<T,3> fftshift(const Array<T,3>& x)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);
  size_t Z = shape(x,2);
  Array<T,3> y(N,M,Z);

  auto l1 = slice(0,N/2);
  auto u1 = slice(N/2,N);
  auto l2 = slice(0,M/2);
  auto u2 = slice(M/2,M);
  auto l3 = slice(0,Z/2);
  auto u3 = slice(Z/2,Z);

  y(l1,l2,l3) = x(u1,u2,u3);
  y(u1,u2,u3) = x(l1,l2,l3);

  y(l1,l2,u3) = x(u1,u2,l3);
  y(u1,u2,l3) = x(l1,l2,u3);

  y(l1,u2,l3) = x(u1,l2,u3);
  y(u1,l2,u3) = x(l1,u2,l3);

  return y;
}

template<class T>
Array<T,3>& fftshift_(Array<T,3>& x)
{
  auto y = fftshift(x);
  x = y;
  return x;
}*/

template<class T>
Array<T>& fftshift_(Array<T>& x)
{

  switch (x.ndims())
  {
    case 1:
    {
      size_t N = x.size();
      if(N % 2 == 0)
      {
          for(size_t n=0; n<N/2; n++)
          {
              std::swap(x(n),x(N/2+n));
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
      break;
    }
    case 2:
    {
      size_t N = shape(x,0);
      size_t M = shape(x,1);

      for(size_t n=0; n<N; n++)
      {
          auto sl = x(n,full);
          fftshift_(sl);
      }

      for(size_t m=0; m<M; m++)
      {
          auto sl = x(full,m);
          fftshift_(sl);
      }
      break;
    }
    default:
      throw std::invalid_argument("fftshift only supported for dim<=2");
    }


  return x;
}

template<class T>
Array<T> fftshift(const Array<T>& x)
{
  auto y = copy(x);
  return fftshift_(y);
}


/*! @} */

}

#endif
