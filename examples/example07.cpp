#include <numcpp.h>
#include <numcpp/wavelet.h>
using namespace numcpp;

int main()
{
  int N = 256;

  auto p = phantom(N);
  export_image(p, "phantom.png");

  auto x = dwt(p, wavelets::Haar);
  export_image(x, "waveletHaar.png");

  auto y = dwt(p, wavelets::Daubechies4);
  export_image(y, "waveletDaubechies4.png");

  auto z = idwt(y, wavelets::Daubechies4);
  export_image(z, "reco.png");
}
/*!
@page example07 Wavelet Transformation Example

@code
int main()
{
  int N = 256;

  auto p = phantom(N);
  export_image(p, "phantom.png");

  auto y = dwt(p, wavelets::Haar);
  export_image(y, "waveletHaar.png");

  auto y = dwt(p, wavelets::Daubechies4);
  export_image(y, "waveletDaubechies4.png");

  auto z = idwt(y, wavelets::Daubechies4);
  export_image(z, "reco.png");
}
@endcode
*/

