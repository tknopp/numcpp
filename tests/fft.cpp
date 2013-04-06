#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/fft/dct", "DCT test" )
{

  {
    size_t N = 8;
    Vector<double> x = range(0,N);

    auto y = dct(x);
    auto z = idct(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    Vector<float> x = range(0,N);

    auto y = dct(x);
    auto z = idct(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

}

