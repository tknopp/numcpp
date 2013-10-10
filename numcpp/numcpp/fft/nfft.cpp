#include "nfft.h"
#include "fft.h"
#include "../core.h"
#include "../base.h"

namespace numcpp
{

template<class T>
Array< std::complex<T> > ndft(const Array<std::complex<T>>& f, const Array<T>& x)
{
  size_t M = shape(x,0);

  Array< std::complex<T> > g = zeros(M);

  for(size_t k=0; k<M; k++)
  {
    Iterator it(f.shape());
    for(size_t l=0; l<f.size(); l++, it++)
    {
      T arg = 0;
      for(int d=0; d<f.ndims(); d++)
        arg += x(k,d)*( (*it)[d] - shape(f,d)/2.0);
      g(k) += f[*it] * exp(-2*pi*I*arg);
    }
  }
  return g;
}

template Array<cdouble> ndft<double>(const Array<cdouble>& f, const Array<double>& x);
//template Array<cfloat> ndft<float>(const Array<cfloat>& f, const Array<float>& x);

template<class T>
Array<std::complex<T>> ndftAdjoint(const Array< std::complex<T>>& fHat, const Array<T>& x,
                                     const std::vector<size_t> &N)
{
  size_t M = shape(x,0);

  Array< std::complex<T> > g = zeros(N);

  for(size_t k=0; k<M; k++)
  {
    Iterator it(g.shape());
    for(size_t l=0; l<g.size(); l++, it++)
    {
      T arg = 0;
      for(int d=0; d<g.ndims(); d++)
        arg += x(k,d)*( (*it)[d] - shape(g,d)/2.0);
      g[*it] += fHat(k) * exp(2*pi*I*arg);
    }
  }
  return g;
}

template Array<cdouble> ndftAdjoint<double>(const Array<cdouble>& fHat, const Array<double>& x, const std::vector<size_t> &N);
//template Array<cfloat> ndftAdjoint<float>(const Array<cfloat>& fHat, const Array<float>& x, const std::vector<size_t> &N);

template<class T>
T nfft_window_hat(T k, size_t n, size_t m, T sigma)
{
  T b = pi*(2-1.0/sigma);
  return besseli0(m*sqrt(b*b-pow(2*pi*k/n,2)));
}

template float nfft_window_hat<float>(float k, size_t n, size_t m, float sigma);
template double nfft_window_hat<double>(double k, size_t n, size_t m, double sigma);


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


template float nfft_window<float>(float x, size_t n, size_t m, float sigma);
template double nfft_window<double>(double x, size_t n, size_t m, double sigma);

template<class T>
NFFTPlan<T>::NFFTPlan(const Array<T>& x, const std::vector<size_t>& N, size_t m, T sigma)
    : D(N.size()), N(N), sigma(sigma), M(shape(x,0)), m(m), x(x), n(D), windowLUT(D), windowHatInvLUT(D)
{
#ifdef NUMCPP_OUTPUT_TIMINGS
    auto t = tic();
#endif

    for(int d=0; d<D; d++)
        n[d] = (size_t)round(sigma*N[d]);

    tmpVec = zeros(n);

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

#ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "Init: ";
    toc(t);
#endif
}

template<class T>
Array<std::complex<T>> NFFTPlan<T>::trafo(const Array< std::complex<T> >& f)
  {
    tmpVec = 0;

    // apodization
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t1 = tic();
    #endif

    apodization<Forward>(const_cast<Array< std::complex<T> >&>(f), tmpVec);

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "Apo:  ";
    toc(t1);
    #endif

    // fft
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t2 = tic();
    #endif

    // fftshifts are implicitely done in convolution and apodization
    fft_(tmpVec, -1);

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "FFT:  ";
    toc(t2);
    #endif

    // convolution
    Array< std::complex<T> > fHat = zeros(M);
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t3 = tic();
    #endif

    convolution<Forward>(tmpVec, fHat);

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "Conv: ";
    toc(t3);
    #endif

    return fHat;
  }

template<class T>
Array< std::complex<T> > NFFTPlan<T>::adjoint(const Array<std::complex<T>>& fHat)
  {
    tmpVec = 0;

    // convolution
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t1 = tic();
    #endif

    convolution<Adjoint>(tmpVec, const_cast<Array<std::complex<T>>&>(fHat));

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "Conv: ";
    toc(t1);
    #endif

    // fft
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t2 = tic();
    #endif

    // fftshifts are implicitely done in convolution and apodization
    fft_(tmpVec, 1);

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "FFT:  ";
    toc(t2);
    #endif

    // apodization
    #ifdef NUMCPP_OUTPUT_TIMINGS
    auto t3 = tic();
    #endif

    Array< std::complex<T> > f = zeros(N);
    apodization<Adjoint>(f, tmpVec);

    #ifdef NUMCPP_OUTPUT_TIMINGS
    std::cout << "Apo:  ";
    toc(t3);
    #endif

    return f;
  }

  template<class T>
  template<int Z>
  void NFFTPlan<T>::apodization(Array< std::complex<T>>& f, Array< std::complex<T>>& g)
  {
    switch (f.ndims())
    {
      case 1:
      {
        for(size_t l=0; l<N[0]; l++)
        {
          if(Z == Forward)
            g( (l + n[0] - N[0] / 2) % n[0] ) = f(l) * windowHatInvLUT[0](l);
          else
            f(l) = g( (l + n[0] - N[0] / 2) % n[0] ) * windowHatInvLUT[0](l);
        }
        break;
      }
      case 2:
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
        break;
      }
      default:
      {
        std::vector<size_t> idx(f.ndims());
        Iterator it(N);

        for(size_t j=0; j<prod(N); j++, it++)
        {
            T windowHatInvLUTProd = 1;
            for(int d=0; d<f.ndims(); d++)
            {
                idx[d] = ( (*it)[d] + n[d] - N[d] / 2 ) % n[d];
                windowHatInvLUTProd *= windowHatInvLUT[d]((*it)[d]);
            }
            if(Z == Forward)
                g[idx] = f[*it] * windowHatInvLUTProd;
            else
                f[*it] = g[idx] * windowHatInvLUTProd;
        }
        break;
      }
    }
  }

  template<class T>
  template<int Z>
  void NFFTPlan<T>::convolution(Array<std::complex<T>>& g, Array< std::complex<T> >& fHat)
  {
    switch (g.ndims())
    {
      case 1:
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
        break;
      }
      case 2:
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
        break;
      }
      default:
      {
        std::vector<ptrdiff_t> l(g.ndims());
        std::vector<size_t> idx(g.ndims()), P(g.ndims());
        std::vector<T> c(g.ndims());
        for(size_t k=0; k<M; k++) // loop over nonequispaced nodes
        {
            for(int d=0; d<g.ndims(); d++)
            {
                c[d] = std::floor(x(k,d)*n[d]);
                P[d] = 2*m + 1;
            }

            Iterator it(P);
            for(size_t j=0; j<prod(P); j++, it++) // loop over nonzero elements
            {
                // Use linear interpolation and a LUT
                for(int d=0; d<g.ndims(); d++)
                {
                    l[d] = c[d]-m+(*it)[d];
                    idx[d] = (l[d]+n[d]) % n[d];
                }

                std::complex<T> tmp;
                if(Z == Forward) tmp = g[idx];
                else tmp = fHat(k);

                for(int d=0; d<g.ndims(); d++)
                {
                    T idx2 = abs(((x(k,d)*n[d] - l[d])/m )*(K-1));
                    size_t idx2L = idx2;
                    tmp *= (windowLUT[d](idx2L) + ( idx2-idx2L ) * (windowLUT[d](idx2L+1) - windowLUT[d](idx2L) ) );
                }

                if(Z == Forward) fHat(k) += tmp;
                else g[idx] += tmp;
            }
        }
        break;
      }
    }
  }


template class NFFTPlan<double>;

}
