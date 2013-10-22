#include <numcpp/core.h>
#include <numcpp/base.h>
#include <numcpp/fft.h>
#include <numcpp/expressions.h>
#include <numcpp/graphics.h>
using namespace numcpp;

int main()
{
  Array<double> x = ones(16);
  auto y = fft(x);
  auto z = reshape(y,4,4);

  print(z);

  auto p = phantom(256);

  export_image(p, "C:\\test.pdf", 0, 1, colormaps::autumn);
}

/*!
@page example01 Simple Example
This is a simple example of the usage of NumCpp. First a vector of 16 elements is initialized with ones.
Then a one-dimensional FFT of x is calculated. Then the vector is reshaped into a 4x4 matrix and printed
to the console.

@code
int main()
{
  Array<double> x = ones(16);
  auto y = fft(x);
  auto z = reshape(y,4,4);

  print(z);
}
@endcode
*/
