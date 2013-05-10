#ifndef NUMCPP_NFFT_H
#define NUMCPP_NFFT_H

#include "fftw.h"
#include "../graphics.h"

namespace numcpp
{

/*class GlobalData
{
public:
  size_t totalAllocatedBytes = 0;
  size_t numAllocations = 0;
  bool cachingEnabled_ = false;
  std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;

  static GlobalData& GetInstance();
};

inline GlobalData &GlobalData::GetInstance()
{
   static GlobalData instance;
   return instance;
}
*/

template<class T>
T nfft_window_hat(T k, size_t n, size_t m, T sigma)
{
  T b = pi*(2-1.0/sigma);
  return besseli0(m*sqrt(b*b-pow(2*pi*k/n,2)));
}

template<class T>
T nfft_window(T x, size_t n, size_t m, T sigma)
{
  T b = pi*(2-1.0/sigma);
  T arg = m*m - n*n*x*x;
  if(abs(x) < m/static_cast<T>(n))
    return sinh(b*sqrt(arg)) / sqrt(arg)/pi;
  else if(abs(x) > m/static_cast<T>(n))
    return 0;//sin(b*sqrt(-m^2+n^2*x^2))/sqrt(-m^2+n^2*x^2)/pi;
  else
    return b / pi;
}

template<class T>
Vector< std::complex<T> > ndftAdjoint(const Vector<std::complex<T>>& fHat, const Vector<T>& x)
{
  size_t N = size(fHat);
  size_t M = size(x);

  Vector< std::complex<T> > g = zeros(N);

  for(size_t n=0; n<N; n++)
  {
    for(size_t k=0; k<M; k++)
    {
      g(n) += fHat(k) * exp(2*pi*I*x(k)*(n-N/2.0) );
    }
  }

  return g;
}

template<class T>
Vector<std::complex<T> > nfftAdjoint(const Vector<std::complex<T>>& fHat, const Vector<T>& x, size_t m=2, T sigma=2.0 )
{
  size_t N = size(fHat);
  size_t n = round(sigma*N);
  size_t M = size(x);
  Vector<T> windowLUT = zeros(2000);
  Vector<T> windowHatInvLUT = zeros(N);
  Vector< std::complex<T> > tmpVec = zeros(n);

  // initialization of LUTs
  for(size_t l=0; l<size(windowLUT); l++)
  {
    T y = (l / static_cast<T>(size(windowLUT)-1) * 2 - 1) * m/static_cast<T>(n) * 2;
    windowLUT(l) = nfft_window(y, n, m, sigma);
  }

  for(size_t k=0; k<N; k++)
  {
    windowHatInvLUT(k) = 1. / nfft_window_hat(k-N/2.0, n, m, sigma);
  }

  auto fig = Figure();
  fig.plot(windowLUT, "blue");
  fig.save("windowLUT.pdf");

  auto fig2 = Figure();
  fig2.plot(windowHatInvLUT, "blue");
  fig2.save("windowHatInvLUT.pdf");

  // convolution
  for(size_t k=0; k<M; k++) // loop over nonequispaced nodes
  {
    size_t c = (size_t) std::floor(x(k)*n);
    for(size_t l=(c-m); l<(c+m+1); l++) // loop over nonzero elements
    {
      size_t idx = (l+n/2) % n;
      // p.tmpVec[idx] += fHat[k] * p.window( p.x[k] - l/p.n , p.n, p.m, p.sigma)

      size_t idx2 = round(((x(k)*n - l)/m/2.0 + 1)*(size(windowLUT)-1)/2.0);
      tmpVec(idx) += fHat(k) * windowLUT(idx2);
    }
  }

  // fft
  tmpVec = fftshift(ifft(fftshift(tmpVec))) * n;

  // apodization
  Vector< std::complex<T> > f = zeros(N);
  size_t offset = (n - N) / 2;

  for(size_t n=0; n<N; n++)
  {
    f(n) = tmpVec(n+offset) * windowHatInvLUT(n);
  }

  return f;
}




}

#endif
