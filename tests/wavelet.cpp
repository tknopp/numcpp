#include "catch.hpp"
#include <numcpp/core.h>
#include <numcpp/base.h>
#include <numcpp/expressions.h>
#include <numcpp/wavelet.h>
using namespace numcpp;

TEST_CASE( "numcpp/fft/wavelet", "Wavelet test" )
{

  {
    size_t N = 8;
    auto x = randn<double>(N);

    auto y = dwt(x, wavelets::Haar,1);
    auto z = idwt(y, wavelets::Haar,1);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    auto x = randn<double>(N);

    auto y = dwt(x, wavelets::Daubechies4);
    auto z = idwt(y, wavelets::Daubechies4);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    auto x = randn<double>(N,N);

    auto y = dwt(x, wavelets::Haar);
    auto z = idwt(y, wavelets::Haar);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    auto x = randn<double>(N,N);

    auto y = dwt(x, wavelets::Daubechies4);
    auto z = idwt(y, wavelets::Daubechies4);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    auto x = randn<double>(32,16);

    auto y = dwt(x, wavelets::Haar);
    auto z = idwt(y, wavelets::Haar);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    auto x = randn<double>(32,16);

    auto y = dwt(x, wavelets::Daubechies4);
    auto z = idwt(y, wavelets::Daubechies4);

    REQUIRE( norm(x - z) < 1e-6 );
  }

}

