  //#define CATCH_CONFIG_MAIN
  #include "catch.hpp"
  #include <numcpp.h>
  using namespace numcpp;

  template<class T>
  void testHdf5()
  {
      Array<T> A(3,3);
      A = range(0,9);

      auto file = h5create("test.h5");
      h5write(A, file, "/testData");
      h5close(file);

      auto B = h5read<T>("test.h5", "/testData");

      REQUIRE( all(A == B) );
  }

  TEST_CASE( "numcpp/io/hdf5", "HDF5 Test" ) {

      SECTION( "Double", "Double Tests")
      {
        testHdf5<double>();
      }
      SECTION( "Float", "Float Tests")
      {
        testHdf5<float>();
      }

      SECTION( "Int", "Int Tests")
      {
        testHdf5<int>();
      }

  }
