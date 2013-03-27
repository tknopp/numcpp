#include <numcpp.h>
using namespace numcpp;

int main()
{
  Vector<double> x = ones(16);
  auto y = fft(x);
  auto z = reshape(y,4,4);

  //print(z);
}

