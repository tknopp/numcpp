#include "catch.hpp"
#include <numcpp.h>
#include <numcpp/linalg.h>

using namespace numcpp;

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

  {
    size_t M = 3;
    size_t N = 3;
    auto A = array<cdouble>({1, 7, 3+I,
                              9, 9, 4,
                              1, 0, 4}, M, N);

    Array<cdouble> x = ones(N)*I;
    auto y = dot(A,x);
    auto r = solve(A, y);

    REQUIRE( norm( r - x ) < 1e-6  );
  }

}
