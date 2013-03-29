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

  export_pdf(kk, "test.pdf");

  auto zz = meshgrid(range(1,4), range(10,15));

  print(zz.first);
  print(zz.second);

  auto p = phantom(128);

  export_pdf(p, "p.pdf");


  export_pdf(flipud(p), "p_ud.pdf", colormaps::autumn);
  export_pdf(fliplr(p), "p_lr.pdf", colormaps::winter);

  export_pdf(rotl90(p), "p_rotl90.pdf");

  return 0;
}

