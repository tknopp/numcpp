#include <numcpp.h>


using namespace numcpp;

int main()
{
  size_t N = 4;
  Matrix<double> x = reshape(range(0,N*N-1),N,N);

  print(x);
  print(transpose(x));

  print(rotl90(x));
  print(rotr90(x));
  print(rot180(x));

  Vector<double> y = range(0,N);

  print(y);
  reverse_(y);
  print(y);
  shuffle_(y);
  print(y);

  auto z = copy(y);
  z(0) = 30;

  print(y);
  print(z);

  h5write(x, "test.h5", "/testV");

  auto kk = h5read<double,2>("test.h5", "/testV");

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

  {
    auto x = linspace(0,1,100);
    auto fig = Figure();

    fig.imshow(phantom(512));

    fig.plot(sin(2*pi*x), "green");
    fig.plot(cos(2*pi*x), "red");
    fig.plot(sin(2*pi*x), cos(2*pi*x), "yellow");

    fig.save("first_plot.pdf");
  }

  return 0;
}

