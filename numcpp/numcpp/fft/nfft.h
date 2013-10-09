#ifndef NUMCPP_NFFT_H
#define NUMCPP_NFFT_H

#include "../core/array.h"

namespace numcpp
{

/*!
@file

@addtogroup fft
@{
*/

template<class T>
Array< std::complex<T> > ndft(const Array<std::complex<T>>& f, const Array<T>& x);

template<class T>
Array<std::complex<T>> ndftAdjoint(const Array< std::complex<T>>& fHat, const Array<T>& x,
                                     const std::vector<size_t> &N);

template<class T>
T nfft_window_hat(T k, size_t n, size_t m, T sigma);

template<class T>
T nfft_window(T x, size_t n, size_t m, T sigma);

/*!
Class for performing Nonequidistant Fast Fourier Transforms (NFFT)
*/
template<class T>
class NFFTPlan
{
public:
  NFFTPlan(const Array<T>& x, const std::vector<size_t>& N, size_t m, T sigma);
  Array<std::complex<T>> trafo(const Array< std::complex<T> >& f);
  Array< std::complex<T> > adjoint(const Array<std::complex<T>>& fHat);

private:

  template<int Z>
  void apodization(Array< std::complex<T>>& f, Array< std::complex<T>>& g);

  template<int Z>
  void convolution(Array<std::complex<T>>& g, Array< std::complex<T> >& fHat);

  int D;
  std::vector<size_t> N;
  double sigma;
  std::vector<size_t> n;
  size_t M;
  size_t m;
  size_t K = 2000;

  const Array<T> x;
  std::vector<Array<T>> windowLUT;
  std::vector<Array<T>> windowHatInvLUT;
  Array< std::complex<T> > tmpVec;

  static const int Forward = 1;
  static const int Adjoint = -1;
};



/*!
Calculate the Nonequidistant Fast Fourier Transforms (NFFT) of the array f. The nonequidistant nodes
are given as a MxD matrix, where M is the number of nodes and D the dimensionality of the transform.

The accuracy of the transform depends on the oversampling factor sigma and the cut-off parameter m.

\sa nfftAdjoint
*/
template<class T>
Array<std::complex<T> > nfft(const Array<std::complex<T>>& f, const Array<T>& x, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T> p(x, f.shape(), m, sigma);
  return p.trafo(f);
}

/*!
Calculate the adjoint Nonequidistant Fast Fourier Transforms (NFFT) of the array f.

\sa nfft
*/
template<class T>
Array<std::complex<T> > nfftAdjoint(const Array<std::complex<T>>& fHat, const Array<T>& x,
                                         std::vector<size_t> N, size_t m=2, T sigma=2.0 )
{
  NFFTPlan<T> p(x, N, m, sigma);
  return p.adjoint(fHat);
}


/*! @} */

}

#endif
