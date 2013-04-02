#ifndef NUMCPP_H
#define NUMCPP_H

#include "numcpp/core.h"
#include "numcpp/base.h"

#include "numcpp/io.h"
#include "numcpp/solvers.h"
#include "numcpp/fft.h"
#include "numcpp/graphics.h"
#include "numcpp/linalg.h"

/*!
@mainpage
Numcpp is a numerical C++ library that uses features of the new C++ standard C++11, which enables a clean implementation

Here is a simple example of the usage of numcpp:

@code
#include <numcpp.h>
using namespace numcpp;

int main()
{
  Vector<double> x = ones(16);
  auto y = fft(x);
  auto z = reshape(y,4,4);

  print(z);
}
@endcode

\section Why

sdfsfd

*/



#endif // NUMCPP_H

