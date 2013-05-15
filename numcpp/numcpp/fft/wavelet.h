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

template<class T, class R, class U>
AbstractStridedVector<T,R>& dwtStep_(AbstractStridedVector<T,R>& x, const Vector<U>& g, const Vector<U>& h)
{
  auto N = size(x);
  auto L = size(g);
  auto y = copy(x);

  for(size_t n=0; n<N/2; n++)
  {
    x(n) = 0; x(n+N/2) = 0;
    for(size_t l=0; l<L; l++)
    {
      x(n) += y( (2*n+l) % N ) * h(L-1-l);
      x(n+N/2) += y( (2*n+l) % N ) * g(L-1-l);
    }
  }
  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& dwtStep_(AbstractStridedMatrix<T,R>& x, const Vector<U>& g, const Vector<U>& h)
{
  size_t N = shape(x,0);
  size_t M = shape(x,1);

  for(size_t n=0; n<N; n++)
  {
    auto sl = x(n,full);
    dwtStep_(sl, g, h);
  }

  for(size_t m=0; m<M; m++)
  {
    auto sl = x(full,m);
    dwtStep_(sl, g, h);
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedVector<T,R>& dwt_(AbstractStridedVector<T,R>& x, const Vector<U>& g, const Vector<U>& h, int steps=-1)
{
  auto N = size(x);

  if(steps == -1)
  {
    steps = log2(N)-1;
  }

  size_t M = N;
  for(size_t k=0; k<steps; k++)
  {
    dwtStep_(x(S{0,M}), g, h);
    M /= 2;
  }

  return x;
}

template<class T, class R, class U>
AbstractStridedMatrix<T,R>& dwt_(AbstractStridedMatrix<T,R>& x, const Vector<U>& g, const Vector<U>& h, int steps=-1)
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
    dwtStep_(sl, g, h);
    M /= 2;
  }

  return x;
}

/*! @} */

}

#endif
