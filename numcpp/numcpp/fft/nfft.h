#ifndef NFFT_H
#define NFFT_H

#include "fftw.h"

namespace numcpp
{

template<class T, class U=double>
class NFFTMatrix
{
public:
  NFFTMatrix(size_t N, Vector<U> x)
    : m(2)
    , n(2*N)
    , sigma(2.0)
    , windowLUT(2000)
    , windowHatInvLUT(N)
    , tmpVec(n)
  {
     for(size_t l=0; l<windowLUT.size(); l++)
     {
       U y = ((l-1) / (windowLUT.size()-1) * 2 - 1) * m/n * 2;
       windowLUT[l] = window_kaiser_bessel(y, n, m, sigma)
      }

  windowHatInvLUT = zeros(Float64, N)
  for y=1:N
    windowHatInvLUT[y] = 1. / window_kaiser_bessel_hat(y-1-N/2, n, m, sigma)
  end

  tmpVec = zeros(Complex128, n)

  NFFTPlan(N, length(x), x, m, sigma, n, window_kaiser_bessel,
      window_kaiser_bessel_hat, Complex128, windowLUT, windowHatInvLUT, tmpVec )

  }

private:
  size_t M;
  size_t N;
  size_t n;
  Vector<U> x;
  U sigma;
  int m;
  Vector<U> windowLUT;
  Vector<U> windowHatInvLUT;
  Vector<T> tmpVec;
};

/*
nfftConvolve (p::NFFTPlan, fHat::Vector{T})
  NLUT = length(p.windowLUT)

  for k=1:p.M # loop over nonequispaced nodes
    c = int(floor(p.x[k]*p.n))
    for l=(c-p.m):(c+p.m+1) # loop over nonzero elements

      idx = mod(l+p.n/2, p.n) + 1

      #p.tmpVec[idx] += fHat[k] * p.window( p.x[k] - l/p.n , p.n, p.m, p.sigma)

      idx2 = round(((p.x[k]*p.n - l)/p.m/2. + 1)*(NLUT-1)/2) +1
      p.tmpVec[idx] += fHat[k] * p.windowLUT[idx2]
    end
  end
end

function apodization{T}(p::NFFTPlan, g::Vector{T})
  f = zeros(T, p.N)
  const offset = (p.n - p.N) / 2
  for y=1:p.N
    f[y] = g[y+offset] * p.windowHatInvLUT[y]
  end
  return f
end
*/
}

#endif
