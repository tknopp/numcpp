#include "radon.h"
#include "../base/initializers.h"
#include "../expressions/arithmetic.h"
#include "../expressions/functions.h"

namespace numcpp
{

template<class T>
inline T _interp2d(const Array<T>& A, T x, T y )
{
  T value;

  if ((abs(x) >= 0.5) || (abs(y) >= 0.5))
      value = 0;
  else
  {
    //index = round((pos./[1, 1]+0.5).*(N-1)) +1;
    size_t ix = round((x+0.5)*shape(A,0)-0.5);
    size_t iy = round((y+0.5)*shape(A,1)-0.5);

    value = A(ix, iy);
  }

  return value;
}

template<class T>
Array<T> radon(const Array<T>& A, const Array<T>& xi, const Array<T>& gamma)
{
  size_t L = 256;

  size_t M = size(xi);
  size_t D = size(gamma);

  Array<T> B = zeros(M, D);

  Array<T> eta = linspace(-0.5,0.5,L)*sqrt(2);

  for(size_t m=0; m<M; m++)
     for(size_t d=0; d<D; d++)
        for(size_t l=0; l<L; l++)
        {
            T x = xi(m)*cos(gamma(d))-eta(l)*sin(gamma(d));
            T y = xi(m)*sin(gamma(d))+eta(l)*cos(gamma(d));

            //std::cout << "x = " << x << " y = " << y << " val = " << _interp2d(A,x,y) << std::endl;

            B(m,d) += _interp2d(A,x,y);
        }

  return B;
}

template Array<double> radon(const Array<double>& A, const Array<double>& xi, const Array<double>& gamma);

template<class T>
Array<T> fbp(const Array<T>& A, const Array<T>& gamma, size_t N)
{
  Array<T> I = zeros(N,N);

  Array<T> x = linspace(-0.5,0.5,N);
  Array<T> y = linspace(-0.5,0.5,N);

  size_t nrDetPix = shape(A,0);
  size_t nrAngles = shape(A,1);

  Array<T> filter = abs( range(-nrDetPix/2, nrDetPix/2) ); //abs(linspace(-nrDetPix/2, nrDetPix/2,nrDetPix))';

  for(size_t t=0; t<size(gamma); t++)
  {
    auto fhat = fftshift(fft(A(full,t)));
    fhat = fhat * filter;
    A(full,t) = real(ifft(fftshift(fhat))); //Todo -> ifftshift
  }

  //gamma = gamma / 360.0 * 2 * pi;

  for(size_t m=0; m<N; m++)
    for(size_t n=0; n<N; n++)
      for(size_t t=0; t<nrAngles; t++)
      {
        T xi = x(m)*cos(gamma(t))+y(n)*sin(gamma(t));
        size_t index = round((xi/sqrt(2.0) + 0.5) * (nrDetPix-1.0));

        I(m,n) += A(index,t);
      }


  //I = real(I/(2*nrAngles)*pi);

  //I = rot90(I); %flipud(fliplr(I));

  return I;
}

template Array<double> fbp(const Array<double>& A, const Array<double>& gamma, size_t N);



}
