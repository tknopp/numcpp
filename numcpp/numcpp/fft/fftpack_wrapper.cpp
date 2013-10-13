
#include "fft.h"
#include "fftpack.h"

namespace numcpp
{

/*Array< std::complex<double> >&
internal_fft2_ (Array< std::complex<double> >& x, int dir)
{
  auto N = x.size();

  int shape_[x.ndims()];

  for(int d=0; d<x.ndims(); d++)
    shape_[d] = shape(x,d);

  double *out = (double*) x.data();
  double *in = out;

  Array<double> wsave(4*max(shape_)+15);

  int ifac[64];

  cffti( shape_[ x.ndims() - 1 ], wsave.data(), ifac );

  if(dir == -1)
    cfftf(shape_, in, wsave.data(), ifac);
  else
    cfftb(shape_, in, wsave.data(), ifac);

  for(int d=0;)


  return x;
}*/


}
