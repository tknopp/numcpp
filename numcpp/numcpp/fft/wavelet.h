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
  const Matrix<double> Haar(  { 1 / sqrt(2), 1 / sqrt(2),
                                1 / sqrt(2), -1 / sqrt(2) }, 2, 2);
  const Matrix<double> Daubechies4({ (1 + sqrt(3)) / (4 * sqrt(2)), (3 + sqrt(3)) / (4 * sqrt(2)), (3 - sqrt(3)) / (4 * sqrt(2)), (1 - sqrt(3)) / (4 * sqrt(2)),
                                     (1 - sqrt(3)) / (4 * sqrt(2)), -(3 - sqrt(3)) / (4 * sqrt(2)), (3 + sqrt(3)) / (4 * sqrt(2)), -(1 + sqrt(3)) / (4 * sqrt(2))}, 2, 4);

}

template<class T, class R, class U>
AbstractStridedVector<T,R>& dwtStep_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  auto N = size(x);
  auto L = shape(wavelet,1);
  auto y = copy(x);

  auto N1 = N / 2;
  auto N2 = N - N1;

  for(size_t n=0; n<N1; n++)
  {
    x(n) = 0;
    for(size_t l=0; l<L; l++)
    {
      x(n) += y( (2*n+l) % N ) * wavelet(0, L-1-l);
    }
  }

  for(size_t n=0; n<N2; n++)
  {
    x(n+N1) = 0;
    for(size_t l=0; l<L; l++)
    {
      x(n+N1) += y( (2*n+l) % N ) * wavelet(1, L-1-l);
    }
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedVector<T,R>& idwtStep_(AbstractStridedVector<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar)
{
  auto N = size(x);
  auto L = shape(wavelet,1);
  auto y = copy(x);

  auto N1 = N / 2;
  auto N2 = N - N1;

  for(ptrdiff_t n=0; n<N2; n++)
  {
    x(2*n) = 0;
    for(ptrdiff_t l=0; l<L/2; l++)
    {
      auto idx1 = (n-l+N1) % (N1);
      auto idx2 = (n-l+N2) % (N2) + N1;
      x(2*n)   += y( idx1 ) * wavelet(0, L-1-2*l)  +  y( idx2 ) * wavelet(1, L-1-2*l);
    }
  }

  for(ptrdiff_t n=0; n<N1; n++)
  {
    x(2*n+1) = 0;
    for(ptrdiff_t l=0; l<L/2; l++)
    {
      auto idx1 = (n-l+N1) % (N1);
      auto idx2 = (n-l+N2) % (N2) + N1;
      x(2*n+1) += y( idx1 ) * wavelet(0, L-2-2*l)  +  y( idx2 ) * wavelet(1, L-2-2*l);
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

  ptrdiff_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M});
    dwtStep_(sl, wavelet);
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

  std::vector<ptrdiff_t> M;
  M.resize(steps);

  M[steps-1] = N;
  for(size_t k=0; k<steps-1; k++)
    M[steps-k-2] = M[steps-k-1] / 2;

  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M[k]});
    idwtStep_(sl, wavelet);
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& dwt_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = std::min(shape(x,0),shape(x,1));

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  ptrdiff_t M1 = shape(x,0);
  ptrdiff_t M2 = shape(x,1);
  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M1},S{0,M2});
    dwtStep_(sl, wavelet);
    M1 /= 2;
    M2 /= 2;
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& idwt_(AbstractStridedMatrix<T,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  auto N = std::min(shape(x,0),shape(x,1));

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  std::vector<ptrdiff_t> M1,M2;
  M1.resize(steps);
  M2.resize(steps);

  M1[steps-1] = shape(x,0);
  M2[steps-1] = shape(x,1);
  for(size_t k=0; k<steps-1; k++)
  {
    M1[steps-k-2] = M1[steps-k-1] / 2;
    M2[steps-k-2] = M2[steps-k-1] / 2;
  }

  for(size_t k=0; k<steps; k++)
  {
    auto sl = x(S{0,M1[k]},S{0,M2[k]});
    idwtStep_(sl, wavelet);
  }

  return x;
}

template<class T, int D, class R, class U>
Array<T,D> dwt(const AbstractArray<T,D,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  Array<T,D> y = copy(x);
  dwt_(y, wavelet, steps);
  return y;
}

template<class T, int D, class R, class U>
Array<T,D> idwt(const AbstractArray<T,D,R>& x, const Matrix<U>& wavelet=wavelets::Haar, int steps=-1)
{
  Array<T,D> y = copy(x);
  idwt_(y, wavelet, steps);
  return y;
}


/*! @} */

}

#endif
