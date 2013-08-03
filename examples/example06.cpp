#include <numcpp.h>
#include <numcpp/tomography.h>
using namespace numcpp;

int main()
{
  size_t N = 256;
  auto x = phantom(N);

  Array<double> xi = linspace(-sqrt(2)/2, sqrt(2)/2, N);
  Array<double> gamma = linspace(0, 2*pi, N);

  auto R = radon(x, xi, gamma);

  export_image(R, "Radon.png");

  auto B = fbp(R, gamma, 256);

  export_image(B, "Reco.png");
}
/*!
@page example06 Radon Transformation Example

@code
int main()
{
  size_t N = 256;
  auto x = phantom(N);

  Array<double> xi = linspace(-sqrt(2)/2, sqrt(2)/2, N);
  Array<double> gamma = linspace(0, 2*pi, N);

  auto R = radon(x, xi, gamma);

  export_image(R, "Radon.png");

  auto B = fbp(R, gamma, 256);

  export_image(B, "Reco.png");
}
@endcode
*/

