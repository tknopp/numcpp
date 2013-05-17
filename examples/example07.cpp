#include <numcpp.h>
using namespace numcpp;

int main()
{

  size_t N = 16;
  Vector<double> x = linspace(-0.5, 0.5, N);

  dwtStep_(x, wavelets::Haar);

  print(x);

  auto p = phantom(255); p +=1;
  auto y = dwt(p, wavelets::Daubechies4, 2);
  export_image(y, "wavelet.png", colormaps::gray);

  auto z = idwt(y, wavelets::Daubechies4, 2);
  export_image(z, "waveletI.png", colormaps::gray);
}

