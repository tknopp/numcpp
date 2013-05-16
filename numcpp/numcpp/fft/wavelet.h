#ifndef NUMCPP_WAVELET_H
#define NUMCPP_WAVELET_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup fft
@{
*/

/// namespace containing predefined wavelets
namespace wavelets
{
  /// Haar Wavelet
  const Matrix<double> Haar = { 1 / sqrt(2), 1 / sqrt(2), 
	                            1 / sqrt(2), -1 / sqrt(2) };
  const Matrix<double> Daubechies4 { (1 + sqrt(3)) / (4 * sqrt(2)), (3 + sqrt(3)) / (4 * sqrt(2)), (3 - sqrt(3)) / (4 * sqrt(2)), (1 - sqrt(3)) / (4 * sqrt(2)),
                                     (1 - sqrt(3)) / (4 * sqrt(2)), (3 - sqrt(3)) / (4 * sqrt(2)), (3 + sqrt(3)) / (4 * sqrt(2)), -(1 + sqrt(3)) / (4 * sqrt(2))};

}

template<class T, class R, class U>
AbstractStridedVector<T,R>& dwtStep_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  auto N = size(x);
  auto L = size(g);
  auto y = copy(x);

  for(size_t n=0; n<N/2; n++)
  {
    x(n) = 0; x(n+N/2) = 0;
    for(size_t l=0; l<L; l++)
    {
      x(n) += y( (2*n+l) % N ) * wavelet(0, L-1-l);
      x(n+N/2) += y( (2*n+l) % N ) * wavelet(1, L-1-l);
    }
  }
  return x;
}

template<class T, class R, class U>
AbstractStridedVector<T,R>& idwtStep_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  auto N = size(x);
  auto L = size(g);
  auto y = copy(x);

  for(size_t n=0; n<N/2; n++)
  {
    x(2*n) = 0; x(2*n+1) = 0;
    for(size_t l=0; l<L/2; l++)
    {
      x(2*n)   += y( (n-l-1+N/2) % (N/2) ) * wavelet(0, L-1-2*l)  +  y( (n-l-1+N/2) % (N/2) + N/2 ) * wavelet(1, L-1-2*l);
      x(2*n+1) += y( (n-l-1+N/2) % (N/2) ) * wavelet(0, L-2-2*l)  +  y( (n-l-1+N/2) % (N/2) + N/2 ) * wavelet(1, L-2-2*l);;
    }
  }
  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& dwtStep_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);

  for(size_t n=0; n<N; n++)
  {
    auto sl = x(n,full);
    dwtStep_(sl, wavelet);
  }

  for(size_t m=0; m<M; m++)
  {
    auto sl = x(full,m);
    dwtStep_(sl, wavelet);
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& idwtStep_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);

  for(size_t n=0; n<N; n++)
  {
    auto sl = x(n,full);
    idwtStep_(sl, wavelet);
  }

  for(size_t m=0; m<M; m++)
  {
    auto sl = x(full,m);
    idwtStep_(sl, wavelet);
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedVector<T,R>& dwt_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = size(x);

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  size_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    dwtStep_(x(S{0,M}), wavelet);
    M /= 2;
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedVector<T,R>& idwt_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = size(x);

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  size_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    idwtStep_(x(S{0,M}), wavelet);
    M /= 2;
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& dwt_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = shape(x,0);

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  size_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M},S{0,M});
    dwtStep_(sl, wavelet);
    M /= 2;
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& idwt_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = shape(x,0);

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  size_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M},S{0,M});
    idwtStep_(sl, wavelet);
    M /= 2;
  }

  return x;
}

/*! @} */

}

#endif
