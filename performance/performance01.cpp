#include <numcpp.h>
#include <Eigen/Core>

using namespace numcpp;

int main()
{
  auto N = 1000000;
  auto M = 10;

  // NumCpp

  auto t = tic();

  for(int l=0;l<10;l++)
  {
    Array<cdouble,1> x = zeros(N);
    for(auto i=0; i<M; i++)
      x += ones(N);

    for(auto i=0; i<N; i++)
      x(i) += i;

    for(auto i=0; i<M; i++)
      x += i*exp(2*pi*I*i*linspace(0,1,N));

    for(auto i=0; i<M; i++)
      x += i;
  }

  toc(t);

  // Eigen

  t = tic();

  for(int l=0;l<10;l++)
  {
    Eigen::ArrayXcd x = Eigen::ArrayXcd::Zero(N);
    for(auto i=0; i<M; i++)
      x += Eigen::ArrayXcd::Ones(N);

    for(auto i=0; i<N; i++)
      x(i) += i;

    for(auto i=0; i<M; i++)
      x += x += i*exp(std::complex<double>(0,1)*Eigen::ArrayXd::LinSpaced(N,0,1)*i*2*pi);

    for(auto i=0; i<M; i++)
      x += i;
  }

  toc(t);

  return 0;
}
