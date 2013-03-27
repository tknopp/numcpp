#include <numcpp.h>


using namespace numcpp;

int main()
{
  size_t N = 4;
  Matrix<double> x = reshape(range(0,N*N-1),N,N);

  print(x);
  print(rotl90(x));
  print(rotr90(x));
  print(rot180(x));

  Vector<double> y = range(0,N);

  print(y);
  reverse_(y);
  print(y);

  auto z = copy(y);
  z(0) = 30;

  print(y);
  print(z);

  print(transpose_(x));

  hdf5write(x, "test.h5", "/testV");

  auto kk = hdf5read<double,2>("test.h5", "/testV");

  print(kk);

  toimage_pdf(kk, "test.pdf", min(kk), max(kk));

  return 0;
}

