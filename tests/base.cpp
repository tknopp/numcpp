#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/base/sum", "Sum test" )
{

  {
    auto x = Vector<double>({1,2,3,4});

    REQUIRE( sum(x) == 10 );
  }

  {
    auto x = Matrix<double>({1,2,3,4},2,2);

    REQUIRE( sum(x) == 10 );
    REQUIRE( all( sum(x,1) == array({3, 7}) ) );
    REQUIRE( all( sum(x,0) == array({4, 6}) ) );
  }

  {
    auto x = array({1,2,3,4});
    auto y = array({1,3,6,10});

    REQUIRE( all( cumsum(x) == y ) );
  }


}

TEST_CASE( "numcpp/base/rounding", "Rounding tests" )
{

  {
    auto x = array({1,2,3,4});
    auto y = array({1.2,2.4,2.5,3.6});

    REQUIRE( all( round(y) == x )  );
  }

}

TEST_CASE( "numcpp/base/shuffle", "Shuffle test" )
{

  {
    auto x = array<int>({1,2,3,4});
    auto y = shuffle(x);

    REQUIRE( sum(x) == sum(y)  );
  }

}

