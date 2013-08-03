#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <numcpp/core.h>
#include <numcpp/base/initializers.h>
#include <numcpp/base/rearrangeArray.h>

using namespace numcpp;

TEST_CASE( "numcpp/core/array/shape", "Array Shape tests" )
{
  Array<double> x(10);

  SECTION( "Array", "Array Tests")
  {
    REQUIRE( x.size() == 10 );
    REQUIRE( shape(x,0) == 10 );
    REQUIRE( x.shape()[0] == 10 );
    REQUIRE( x.shape()[0] == 10 );
    REQUIRE( ndims(x) == 1 );
  }

  Array<double> A(10,10);

  SECTION( "Array", "Array Tests")
  {
    REQUIRE( A.size() == 100 );
    REQUIRE( shape(A,0) == 10 );
    REQUIRE( shape(A,1) == 10 );
    REQUIRE( ndims(A) == 2 );
  }
}

/*TEST_CASE( "numcpp/core/array/order", "Array order test" ) {

  {
    double x [] = {1,2,3,4};
    Array<double> y(x, false, 2, 2);
    Array<double, COLUMN_MAJOR_ORDER> z(x, false, 2, 2);

    REQUIRE( z(0,0) == y(0,0) );
    REQUIRE( z(1,0) == y(0,1) );
    REQUIRE( z(0,1) == y(1,0) );
    REQUIRE( z(1,1) == y(1,1) );
  }
}

TEST_CASE( "numcpp/core/array/nonregularindices", "Non-regular indexing test" ) {

  {
    auto x = eval ( range(0,10) );
    auto idx = V{0, 2, 7};

    auto y = x(idx);

    REQUIRE( y(0) == x(idx[0]) );
    REQUIRE( y(1) == x(idx[1]) );
    REQUIRE( y(2) == x(idx[2]) );
  }

  {
    auto x = reshape( range(0,25), 5, 5 );

    auto y = x(V{0, 2, 3}, 0);

    auto z = x(V{0, 2, 3}, S{0,4});

    auto u = x(V{0, 2, 3}, full);


    //REQUIRE( y(0) == x(0) );
    //REQUIRE( y(1) == x(2) );
    //REQUIRE( y(2) == x(7) );
  }

}*/

TEST_CASE( "numcpp/core/array/external", "Array external data test" ) {

  SECTION( "NoOwnership", "Don't take the ownership of the memory")
  {
    double x [] = {1,2,3,4};
    Array<double> y(x, false, 2, 2);

    REQUIRE( x[0] == y(0,0) );
    REQUIRE( x[1] == y(0,1) );
    REQUIRE( x[2] == y(1,0) );
    REQUIRE( x[3] == y(1,1) );
  }

  SECTION( "WithOwnership", "Take the ownership of the memory")
  {
    int* x = new int[4] {1,2,3,4};
    Array<int> y(x, false, 2, 2);

    REQUIRE( x[0] == y(0,0) );
    REQUIRE( x[1] == y(0,1) );
    REQUIRE( x[2] == y(1,0) );
    REQUIRE( x[3] == y(1,1) );
  }

}

TEST_CASE( "numcpp/core/array/logical", "Logical array operators" )
{
  Array<bool> x = {true, false};
  Array<bool> y = {false, true};

  {
    REQUIRE_FALSE( all(x && y) );
    REQUIRE( all(x || y) );
  }

}

TEST_CASE( "numcpp/core/array/angle", "Angle function" )
{
  Array<cdouble> x = {1, -1, I, -I};
  Array<double> y = {0, pi, pi/2, -pi/2.};

  {
    REQUIRE( all( angle(x) == y ) );
  }

}
