#include <numcpp.h>


using namespace numcpp;


/*
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

  print(  x[{ r(0,2), 1 }] );

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
*/

/*
void testArray()
{
  auto x = array({1,2,3,4});
  auto y = array<cdouble>({1,2,3,4}).reshape({2,2});

  print(x);
  print(y);

}


/*
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

}*/

void testNewArray()
{
  Vector<double> x(10);

  for(size_t i=0; i<10; i++)
    x[i] = i+1.1;

  Vector<double> y = tan((x*x + x - x*x) / x);

  print(x);
  print(y);

  Vector<cdouble> u = zeros(10);
  u = 1+I;

  Vector<cdouble> o = y+u;

  print(o);
  print(u);

  ////

  Matrix<cdouble> z = zeros<2>({9,2});
  Matrix<cdouble> r = zeros(9,2);
  auto ll = range(0,10);

  //auto shape = {1,2,3};
  //Vector<cdouble> rr = zeros(shape);

  print(z);
  print(ll);
  print(max(x));
}

void testArrayFunctions()
{
  Vector<double> x = range(0,10);
  Vector<cdouble> y = I + range(0,10);
  Matrix<double> z = reshape(range(0,16),4,4);

  print(x);
  print(y);

  print("max:");
  print(max(x));
  print("min:");
  print(min(x));

  print("sum:");
  print(sum(x));
  print(sum(y));

  print("mean:");
  print(mean(x));
  print(mean(y));

  print("abs:");
  print(abs(x));
  print(abs(y));

  print("norm:");
  print(norm(x));
  print(norm(y));

  print("conj:");
  print(conj(x));
  print(conj(y));

  print("diag:");
  print(diag(x));
  print(diag(diag(x)));

  print("diff:");
  print(diff(x));
  print(diff(x,2));
  print(diff(x,3));
  print(z);
  print(diff(z,1,1));
  print(diff(z,1,0));

  print("fftshift:");
  print(fftshift(x));
  print(z);
  print(fftshift(z));

}

void testMultiIndex()
{
  Array<double,2> x = zeros(3,3);

  for(size_t i=0; i<x.shape(0); i++)
    for(size_t j=0; j<x.shape(1); j++)
      x[{i,j}] = 2*i+j;
      // x[{i,all,slice(0,end)}] x[]

  print(x);
  auto y = sum(x,0);

  std::cout << "sum\n";
  print(y);


  print( x(slice(0,2), slice(0,2))  );

  print( x(slice(0,end,2), slice(0,end,2))  );

  print( x(full, 0)  );

  print( x(full, 1)  );

  print( x(0,full)  );

  print( x(1,full)  );

  print( x(2, 2)  );

}

void testDot()
{
  size_t M = 3;
  size_t N = 3;
  auto A = array<double>({1, 7, 3, 9, 9, 4, 1, 0, 4},3,3);


  Vector<double> x = ones(N);
  Vector<double> y = dot(A,x);

  print(A);
  print(x);
  print(y);


  auto r = solve(A, y);

  print(r);
  print(dot(A,r));

}

void testFFT()
{
  Vector<cdouble> x = range(0,10);

  print( x );
  print( fft(x) );
  print( ifft(fft(x)) );


  Matrix<cdouble> y = ones(6,6);

  print( y );
  print( fft(y) );
  print( ifft(fft(y)) );

  auto F = FFTMatrix<cdouble,2>(6,6);
  auto z = reshape(y,y.size());

  print( dot(F,z) );
  print( solve(F,dot(F,z)) );

}

void testRandn()
{
  Vector<double> x = randn<double>(10);
  Matrix<double> y = randn<double>(5,4);
  Vector<double> z = randn<double>(10000);

  print(x);
  print(y);
  print(mean(x));
  print(mean(y));
  print(mean(z));

}



/*void testSVD()
{
  Matrix<double> A = Matrix<double>({1,2,3,4,5,6,7,8,9}, 3, 3);
  auto data = svd(A);

  print(A);
  print(data.U);
  print(data.S);
  print(data.V);
}*/

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
  ////testViews1();
  //testViews2();

  //testArray();
  //testSum();
  //testTrig();
  //testDiff();
  //testComparison();

  //testMemoryBlock();

  //std::cout << shapeOfNDVector({{{1,3},{2,3},{3,3}},{{1,3},{2,3},{3,3}}}) << std::endl;
  //std::cout << _array<double>({{1,2,3},{1,2,3}}) << std::endl;

  //testNewArray();
  //testArrayFunctions();
  //testMultiIndex();
  testDot();
  //testFFT();
  //testRandn();
  //testExternalData();
  //testSVD();


  return 0;
}
