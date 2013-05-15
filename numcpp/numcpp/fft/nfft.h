#ifndef NUMCPP_NFFT_H
#define NUMCPP_NFFT_H

#include "fftw.h"
#include "../graphics.h"

namespace numcpp
{

template<class T, int D>
Vector< std::complex<T> > ndft(const Array<std::complex<T>,D>& f, const Matrix<T>& x)
{
  size_t M = shape(x,0);

  Vector< std::complex<T> > g = zeros(M);

  for(size_t k=0; k<M; k++)
  {
    Iterator<D> it(f.shape());
    for(size_t l=0; l<f.size(); l++, it++)
    {
      T arg = 0;
      for(int d=0; d<D; d++)
        arg += x(k,d)*( (*it)[d] - shape(f,d)/2.0);
      g(k) += f[*it] * exp(-2*pi*I*arg);
    }
  }
  return g;
}

template<class T>
Vector< std::complex<T> > ndft(const Vector<std::complex<T>>& f, const Vector<T>& x)
{
  return ndft(f, reshape(x,size(x),1));
}

template<size_t D, class T>
Array<std::complex<T>,D> ndftAdjoint(const Vector< std::complex<T>>& fHat, const Matrix<T>& x,
                                     const std::array<size_t,D> &N)
{
  size_t M = shape(x,0);

  Array< std::complex<T>, D > g = zeros<D>(N);

  for(size_t k=0; k<M; k++)
  {
    Iterator<D> it(g.shape());
    for(size_t l=0; l<g.size(); l++, it++)
    {
      T arg = 0;
      for(int d=0; d<D; d++)
        arg += x(k,d)*( (*it)[d] - shape(g,d)/2.0);
      g[*it] += fHat(k) * exp(2*pi*I*arg);
    }
  }
  return g;
}

template<class T>
Vector< std::complex<T> > ndftAdjoint(const Vector<std::complex<T>>& fHat, const Vector<T>& x, size_t N)
{
  return ndftAdjoint<1>(fHat, reshape(x,size(x),1), {N});
}

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

template<class T, int D>
class NFFTPlan
{
public:

  NFFTPlan(const Matrix<T>& x, std::array<size_t,D> N, size_t m, T sigma)
   : N(N), sigma(sigma), M(shape(x,0)), m(m), x(x)
  {
    auto t = tic();

    for(int d=0; d<D; d++)
      n[d] = (size_t)round(sigma*N[d]);

    tmpVec = zeros<D>(n);

    // initialization of LUTs
    for(int d=0; d<D; d++)
    {
      windowLUT[d] = zeros(3*K/2);
      #ifdef _OPENMP
      #pragma omp parallel for
      #endif
      for(size_t l=0; l<size(windowLUT[d]); l++)
      {
        T y = (l / static_cast<T>(K-1) ) * m/static_cast<T>(n[d]) ;
        windowLUT[d](l) = nfft_window(y, n[d], m, sigma);
      }
    }

    for(int d=0; d<D; d++)
    {
      windowHatInvLUT[d] = zeros(N[d]);
      #ifdef _OPENMP
      #pragma omp parallel for
      #endif
      for(size_t k=0; k<N[0]; k++)
      {
        windowHatInvLUT[d](k) = 1. / nfft_window_hat(k-N[d]/2.0, n[d], m, sigma);
      }
    }

    toc(t);
  }

  NFFTPlan(const Vector<T>& x, size_t N, size_t m, T sigma)
   : NFFTPlan(reshape(x,x.size(),1), {N}, m, sigma)
  {
  }

  Vector<std::complex<T>> trafo(const Array< std::complex<T>, D>& f)
  {
    tmpVec = 0;

    // apodization
    auto t1 = tic();
    apodization<Forward>(const_cast<Array< std::complex<T>, D>&>(f), tmpVec);
    std::cout << "Apo: ";
    toc(t1);

    // fft
    auto t2 = tic();
    // fftshifts are implicitely done in convolution and apodization
    fft_(tmpVec, -1);
    std::cout << "FFT: ";
    toc(t2);

    // convolution
    Vector< std::complex<T> > fHat = zeros(M);
    auto t3 = tic();
    convolution<Forward>(tmpVec, fHat);
    std::cout << "Con: ";
    toc(t3);
    std::cout << "All: ";
    toc(t1);

    return fHat;
  }

  Array< std::complex<T>, D > adjoint(const Vector<std::complex<T>>& fHat)
  {
    tmpVec = 0;

    // convolution
    auto t1 = tic();
    convolution<Adjoint>(tmpVec, const_cast<Vector<std::complex<T>>&>(fHat));
    std::cout << "Con: ";
    toc(t1);

    // fft
    auto t2 = tic();
    // fftshifts are implicitely done in convolution and apodization
    fft_(tmpVec, 1);
    std::cout << "FFT: ";
    toc(t2);

    // apodization
    auto t3 = tic();
    Array< std::complex<T>, D > f = zeros<D>(N);
    apodization<Adjoint>(f, tmpVec);
    std::cout << "Apo: ";
    toc(t3);
    std::cout << "All: ";
    toc(t1);

    return f;
  }


private:

  template<int Z>
  void apodization(Vector< std::complex<T>>& f, Vector< std::complex<T>>& g)
  {
      for(size_t l=0; l<N[0]; l++)
      {
        if(Z == Forward)
          g( (l + n[0] - N[0] / 2) % n[0] ) = f(l) * windowHatInvLUT[0](l);
        else
          f(l) = g( (l + n[0] - N[0] / 2) % n[0] ) * windowHatInvLUT[0](l);
      }
  }

  template<int Z>
  void apodization(Matrix< std::complex<T>>& f, Matrix< std::complex<T>>& g)
  {
      for(size_t lx=0; lx<N[0]; lx++)
      {
        for(size_t ly=0; ly<N[1]; ly++)
        {
          if(Z == Forward)
           g( (lx + n[0] - N[0] / 2) % n[0] , (ly + n[1] - N[1] / 2) % n[1] ) =
             f(lx,ly) * windowHatInvLUT[0](lx) * windowHatInvLUT[1](ly);
          else
           f(lx,ly) = g( (lx + n[0] - N[0] / 2) % n[0] , (ly + n[1] - N[1] / 2) % n[1] )
              * windowHatInvLUT[0](lx) * windowHatInvLUT[1](ly);
        }
      }
  }


  template<int Z, int O>
  void apodization(Array< std::complex<T>,O>& f, Array< std::complex<T>,O>& g)
  {
      std::array<size_t, O> idx;
      Iterator<O> it(N);

      for(size_t j=0; j<prod(N); j++, it++)
      {
        T windowHatInvLUTProd = 1;
        for(int d=0; d<O; d++)
        {
          idx[d] = ( (*it)[d] + n[d] - N[d] / 2 ) % n[d];
          windowHatInvLUTProd *= windowHatInvLUT[d]((*it)[d]);
        }
        if(Z == Forward)
          g[idx] = f[*it] * windowHatInvLUTProd;
        else
          f[*it] = g[idx] * windowHatInvLUTProd;
      }
  }

  template<int Z>
  void convolution(Vector<std::complex<T>>& g, Vector< std::complex<T> >& fHat)
  {
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t k=0; k<M; k++) // loop over nonequispaced nodes
    {
      auto c = std::floor(x(k,0)*n[0]);
      for(ptrdiff_t l=(c-m); l<(c+m+1); l++) // loop over nonzero elements
      {
        ptrdiff_t idx = (l+n[0]) % n[0];

        // Direct evaluation of window:
        //tmpVec(idx) += fHat(k) * nfft_window(x(k) - l/static_cast<T>(n), n, m, sigma);

        // Use linear interpolation and a LUT
        T idx2 = abs(((x(k,0)*n[0] - l)/m )*(K-1));
        size_t idx2L = idx2;

        if(Z == Forward) // Dead code will be eliminated
          fHat(k) += g(idx) * (windowLUT[0](idx2L) + ( idx2-idx2L ) * (windowLUT[0](idx2L+1) - windowLUT[0](idx2L) ) );
        else
          g(idx) += fHat(k) * (windowLUT[0](idx2L) + ( idx2-idx2L ) * (windowLUT[0](idx2L+1) - windowLUT[0](idx2L) ) );
      }
    }
  }

  template<int Z>
  void convolution(Matrix<std::complex<T>>& g, Vector< std::complex<T> >& fHat)
  {
      T scale = 1.0 / m*(K-1);

      #ifdef _OPENMP
      #pragma omp parallel for
      #endif
      for(size_t k=0; k<M; k++) // loop over nonequispaced nodes
      {
        T c0 = std::floor(x(k,0)*n[0]);
        T c1 = std::floor(x(k,1)*n[1]);

        for(ptrdiff_t l0=(c0-m); l0<(c0+m+1); l0++) // loop over nonzero elements
        {
          size_t idx0 = (l0+n[0]) % n[0];

          T idx2 = abs((x(k,0)*n[0] - l0)*scale);
          size_t idx2L = idx2;

          if(Z == Forward)
          {
            auto tmpWin = (windowLUT[0](idx2L) + ( idx2-idx2L ) * (windowLUT[0](idx2L+1) - windowLUT[0](idx2L) ) );

            for(ptrdiff_t l1=(c1-m); l1<(c1+m+1); l1++)
            {
              size_t idx1 = (l1+n[1]) % n[1];
              idx2 = abs((x(k,1)*n[1] - l1)*scale);
              idx2L = idx2;
              fHat(k) += g(idx0,idx1) * tmpWin * (windowLUT[1](idx2L) + ( idx2-idx2L ) * (windowLUT[1](idx2L+1) - windowLUT[1](idx2L) ) );
            }
          } else
          {

            auto tmp = fHat(k) * (windowLUT[0](idx2L) + ( idx2-idx2L ) * (windowLUT[0](idx2L+1) - windowLUT[0](idx2L) ) );

            for(ptrdiff_t l1=(c1-m); l1<(c1+m+1); l1++)
            {
              size_t idx1 = (l1+n[1]) % n[1];
              idx2 = abs((x(k,1)*n[1] - l1)*scale);
              idx2L = idx2;
              g(idx0,idx1) += tmp * (windowLUT[1](idx2L) + ( idx2-idx2L ) * (windowLUT[1](idx2L+1) - windowLUT[1](idx2L) ) );
            }
          }

        }
      }
    }

  template<int Z, int O>
  void convolution(Array< std::complex<T>,O>& g, Vector< std::complex<T> >& fHat)
  {
      std::array<ptrdiff_t,O> l;
      std::array<size_t, O> idx, P;
      std::array<T, O> c;
      for(size_t k=0; k<M; k++) // loop over nonequispaced nodes
      {
        for(int d=0; d<O; d++)
        {
          c[d] = std::floor(x(k,d)*n[d]);
          P[d] = 2*m + 1;
        }

        Iterator<D> it(P);
        for(size_t j=0; j<prod(P); j++, it++) // loop over nonzero elements
        {
          // Use linear interpolation and a LUT
          for(int d=0; d<O; d++)
          {
            l[d] = c[d]-m+(*it)[d];
            idx[d] = (l[d]+n[d]) % n[d];
          }

          std::complex<T> tmp;
          if(Z == Forward) tmp = g[idx];
          else tmp = fHat(k);

          for(int d=0; d<O; d++)
          {
            T idx2 = abs(((x(k,d)*n[d] - l[d])/m )*(K-1));
            size_t idx2L = idx2;
            tmp *= (windowLUT[d](idx2L) + ( idx2-idx2L ) * (windowLUT[d](idx2L+1) - windowLUT[d](idx2L) ) );
          }

          if(Z == Forward) fHat(k) += tmp;
          else g[idx] += tmp;
        }
      }
  }

  std::array<size_t,D> N;
  double sigma;
  std::array<size_t,D> n;
  size_t M;
  size_t m;
  size_t K = 2000;

  const Matrix<T> x;
  std::array<Vector<T>,D> windowLUT;
  std::array<Vector<T>,D> windowHatInvLUT;
  Array< std::complex<T>, D > tmpVec;

  static const int Forward = 1;
  static const int Adjoint = -1;
};





template<class T>
Vector<std::complex<T> > nfft(const Vector<std::complex<T>>& f, const Vector<T>& x, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,1> p(x, size(f), m, sigma);
  return p.trafo(f);
}

template<class T>
Vector<std::complex<T> > nfft(const Matrix<std::complex<T>>& f, const Matrix<T>& x, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,2> p(x, f.shape(), m, sigma);
  return p.trafo(f);
}

template<class T, int D>
Vector<std::complex<T> > nfft(const Array<std::complex<T>,D>& f, const Matrix<T>& x, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,D> p(x, f.shape(), m, sigma);
  return p.trafo(f);
}

template<class T>
Vector<std::complex<T> > nfftAdjoint(const Vector<std::complex<T>>& fHat, const Vector<T>& x, size_t N, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,1> p(x, N, m, sigma);
  return p.adjoint(fHat);
}

template<class T>
Matrix<std::complex<T> > nfftAdjoint(const Vector<std::complex<T>>& fHat, const Matrix<T>& x, std::array<size_t,2> N,
                                     size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,2> p(x, N, m, sigma);
  return p.adjoint(fHat);
}

template<size_t D, class T>
Array<std::complex<T>,D > nfftAdjoint(const Vector<std::complex<T>>& fHat, const Matrix<T>& x, std::array<size_t,D> N,
                                     size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T,D> p(x, N, m, sigma);
  return p.adjoint(fHat);
}

}

#endif
