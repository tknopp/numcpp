#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/sparse/sparseMatrix", "Sparse matrix test" )
{

  {
    auto data = randn<double>(3);
    Vector<size_t> index = { 0, 1, 2 };
    Vector<size_t> ptr = { 0, 1, 2, 3  };

    SparseMatrixCRS<double> A(data, index, ptr, 3, 3);

    Vector<double> x = ones(3);

    auto y = dot(A, x);
    auto r = cgnr(A, y, 3);

    REQUIRE( norm(x - r) < 1e-6 );
  }

  {
    auto A = Matrix<double>({0, 1, 0,
                             2, 0, 4,
                             1, 0, 0}, 3, 3);

    Vector<double> x = ones(3);

    auto y = dot(A, x);

    //print(y);

    SparseMatrixCRS<double> B(A, 0.1);

    auto z = dot(B, x);

    //print(z);

    REQUIRE( norm(y - z) < 1e-6 );


    //std::cout << "norm(x,y) = " << norm( B(1,full) ) << std::endl;

  }

}

TEST_CASE( "numcpp/sparse/sparseVector", "Sparse vector test" )
{

  {
    Vector<double> data = {1, 3, 5};
    Vector<size_t> index = { 0, 2, 3 };

    SparseVector<double> x(data, index, 6);

    Vector<double> y = 2*ones(5);

    //std::cout << "dot(x,y) = " << dot(x, y) << std::endl;
    //std::cout << "norm(x,y) = " << norm(x) << std::endl;
  }
}
