#include <numcpp.h>
using namespace numcpp;

int main()
{

  size_t N = 16;
  Vector<double> x = linspace(-0.5, 0.5, N);
  Vector<double> h = { 1 / sqrt(2), 1 / sqrt(2) };
  Vector<double> g = { 1 / sqrt(2), -1 / sqrt(2) };

  dwtStep_(x, g, h);

  print(x);

  auto p = phantom(256);
  dwt_(p, g, h);
  export_image(p, "wavelet.pdf");
}

