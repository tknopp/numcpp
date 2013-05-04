#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/solvers/kaczmarz", "Kaczmarz test" )
{

  {
    size_t M = 3;
    size_t N = 3;
    auto A = array<double>({1, 7, 3,
                            9, 9, 4,
                            1, 0, 4}, M, N);

    auto x = eval(ones(N));
    auto y = dot(A,x);
    auto r = kaczmarz(A, y, 100);

    REQUIRE( norm( r - x ) < 1e-6  );
  }

}

TEST_CASE( "numcpp/solvers/cgnr", "CGNR test" )
{
  {
    size_t M = 3;
    size_t N = 3;
    auto A = Matrix<double>({1, 7, 3,
                             9, 9, 4,
                             1, 0, 4}, M, N);

    Vector<double> x = ones(N);
    auto y = dot(A,x);
    auto r = cgnr(A, y, 3);

    print(r);

    REQUIRE( norm( r - x ) < 1e-6  );
  }

  {
    size_t M = 3;
    size_t N = 3;
    auto A = Matrix<cdouble>({1, 7, 3+I,
                              9, 9, 4,
                              1, 0, 4}, M, N);

    Vector<cdouble> x = ones(N)*I;
    auto y = dot(A,x);
    auto r = cgnr(A, y, 3);

    print(y);
    print(r);

    REQUIRE( norm( r - x ) < 1e-6  );
  }

}

