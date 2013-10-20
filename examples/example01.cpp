#include <numcpp/core.h>
#include <numcpp/base.h>
#include <numcpp/fft.h>
#include <numcpp/expressions.h>
using namespace numcpp;

int main()
{
  Array<double> y = 2*ones(16);

  y += pow(y, 2);
  //auto y = fft(x);
  auto z = reshape(y,4,4);

  print(z);
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
