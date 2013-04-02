#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/base/sum", "Sum test" )
{

  {
    auto x = array({1,2,3,4});

    REQUIRE( sum(x) == 10 );
  }

  {
    auto x = array({1,2,3,4},2,2);

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

TEST_CASE( "numcpp/linalg/solve", "Solve test" )
{

  {
    size_t M = 3;
    size_t N = 3;
    auto A = array<double>({1, 7, 3,
                            9, 9, 4,
                            1, 0, 4}, M, N);

    auto x = eval(ones(N));
    auto y = dot(A,x);
    auto r = solve(A, y);

    REQUIRE( norm( r - x ) < 1e-6  );
  }

}

