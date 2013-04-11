#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/core/array/shape", "Array Shape tests" )
{
  Vector<double> x(10);

  SECTION( "Vector", "Vector Tests")
  {
    REQUIRE( x.size() == 10 );
    REQUIRE( x.shape(0) == 10 );
    REQUIRE( x.shape()[0] == 10 );
    REQUIRE( x.shape()[0] == 10 );
    REQUIRE( x.ndims() == 1 );
  }

  Matrix<double> A(10,10);

  SECTION( "Matrix", "Matrix Tests")
  {
    REQUIRE( A.size() == 100 );
    REQUIRE( A.shape(0) == 10 );
    REQUIRE( A.shape(1) == 10 );
    REQUIRE( A.ndims() == 2 );
  }
}

TEST_CASE( "numcpp/core/array/order", "Array order test" ) {

  {
    double x [] = {1,2,3,4};
    Matrix<double> y(x, false, 2, 2);
    Matrix<double, COLUMN_MAJOR_ORDER> z(x, false, 2, 2);

    REQUIRE( z(0,0) == y(0,0) );
    REQUIRE( z(1,0) == y(0,1) );
    REQUIRE( z(0,1) == y(1,0) );
    REQUIRE( z(1,1) == y(1,1) );
  }
}

TEST_CASE( "numcpp/core/array/nonregularindices", "Non-regular indexing test" ) {

  {
    auto x = eval ( range(0,10) );

    auto idx = {0, 2, 7};
    auto y = x(idx);

    print(y);

    //auto p = x({0, 2, 7, 6});

    REQUIRE( y(0) == x(0) );
    REQUIRE( y(1) == x(2) );
    REQUIRE( y(2) == x(7) );
  }
}

TEST_CASE( "numcpp/core/array/external", "Array external data test" ) {

  SECTION( "NoOwnership", "Don't take the ownership of the memory")
  {
    double x [] = {1,2,3,4};
    Matrix<double> y(x, false, 2, 2);

    REQUIRE( x[0] == y(0,0) );
    REQUIRE( x[1] == y(0,1) );
    REQUIRE( x[2] == y(1,0) );
    REQUIRE( x[3] == y(1,1) );
  }

  SECTION( "WithOwnership", "Take the ownership of the memory")
  {
    int* x = new int[4] {1,2,3,4};
    Matrix<int> y(x, false, 2, 2);

    REQUIRE( x[0] == y(0,0) );
    REQUIRE( x[1] == y(0,1) );
    REQUIRE( x[2] == y(1,0) );
    REQUIRE( x[3] == y(1,1) );
  }

}

TEST_CASE( "numcpp/core/array/logical", "Logical array operators" )
{
  auto x = array<bool>({true, false});
  auto y = array<bool>({false, true});

  {
    REQUIRE_FALSE( all(x && y) );
    REQUIRE( all(x || y) );
  }

}

TEST_CASE( "numcpp/core/array/angle", "Angle function" )
{
  auto x = array<cdouble>({1, -1, I, -I});
  auto y = array<double>({0, pi, pi/2, -pi/2.});

  {
    REQUIRE( all( angle(x) == y ) );
  }

}
