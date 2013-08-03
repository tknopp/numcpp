#include "../numcpp/numcpp.h"

//#include <valarray>




using namespace numcpp;

void test1()
{
  auto x = arange(0,18,2).reshape({3,3});

  auto y = x.copy();

  print(x);
  print(y);
}

void testViews1()
{
  auto x = arange(9).reshape({3,3});

  print( x({{0,end}, {0,end}}) );

  print( x({ {0,-1,2}, {0,-1,2} }) );

  print( x({ {0,2}, {0,2} }) );

  print( x({ {0,2}, 1}) );
  print( x({ {0,2}, 1}) );

  print( x( r(0,2), r(0,2) ) );
  print( x( 1, 1 ) );

}

void testViews2()
{
  auto x = ones<cdouble>(10) * cdouble(3,4) * cdouble(3,4);
  auto y = ones<cdouble>({2,1})*10;


  print( x );

  x( Range(3,5) ) = y;

  print( x );

}

void testAdd()
{
  auto x = arange(3);

  auto y = 2*ones<cdouble>(3) + 4*ones(3);

  print( x );
  print( y );
  print( x+y );
  print( x + 10.0f );
  print( 10 + x );
  print( y + x + y * x + 2 );
}

void testMult()
{
  auto x = arange(3);

  auto y = 2*ones<cdouble>(3) + 4*ones(3);

  print( x*y );
  print( 10*x );
  print( x*10 );

}

void testComparison()
{
  auto x = arange(5);

  print( x > 3 );

  print( (x >= 2)*x );

}

void testEye()
{
  auto x = eye(4);
  print(x);
  auto y = eye<cdouble>(4);
  print(y);
  auto z = eye<int>(4);
  print(z);
}

void testLinspace()
{
  auto x = linspace(0,10,7);
  print(x);
}

void testRealImag()
{
  auto x = linspace(0,10,7);
  print(x.real());
  print(x.imag());

  auto y = zeros<cdouble>({5});
  y = cdouble(1,3);

  print(y.real());
  print(y.imag());
}


void testArray()
{
  auto x = array({1,2,3,4});
  auto y = array<cdouble>({1,2,3,4}).reshape({2,2});

  print(x);
  print(y);

}

void testSum()
{
  auto x = array({1,2,3,4});
  auto y = array<cdouble>({1,2,3,4}).reshape({2,2});

  print(sum(x));
  print(sum(y));
  print(sum(y,1));
  print(sum(y,0));
}

void testTrig()
{
  auto x = linspace(0, 1, 10);

  print( x );
  print( 2*pi*I*x );
  print( exp(2*pi*I*x) );

}

void testDiff()
{
  print( diff(arange(0, 10)) );

  auto x = arange(0, 25).reshape({5,5});
  print( diff(x) );
  print( diff(x, 1) );

}

void testMemoryBlock()
{
    //auto x = arange(3);

    std::cout << "tic" << std::endl;

    auto y = 13*ones<double>(3)*4 +  4*ones<cdouble>(3)*2;

    auto z = pow(y,2);
    //auto z = pow(acos(cos(y)),2);

    std::cout << "toc" << std::endl;

    std::cout << y;
    std::cout << z;

    //print( y + x + y * x + 2 );

    //std::cout << x;
}

void testFile()
{
  std::cout << "---- testFile ----" << std::endl;
  auto x = arange(0, 9).reshape({3,3});
  print( x );
  tofile(x, "/tmp/testFile.dat");
  tofile(x, "/tmp/testFile.txt", " ");
  auto y = fromfile<int>("/tmp/testFile.dat");
  print( y );

}

int main()
{
  std::cout << "Unit Test NumCpp" << std::endl;

  //test1();
  //test2();
  //testAdd();
  //testMult();
  //testEye();

  //testLinspace();
  //testRealImag();
  testViews1();
  //testViews2();

  //testArray();
  //testSum();
  //testTrig();
  //testDiff();
  //testComparison();

  //testMemoryBlock();

  testFile();

  //std::cout << shapeOfNDVector({{{1,3},{2,3},{3,3}},{{1,3},{2,3},{3,3}}}) << std::endl;
  //std::cout << _array<double>({{1,2,3},{1,2,3}}) << std::endl;

  return 0;
}
