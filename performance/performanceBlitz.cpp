#include <blitz/array.h>
#include <random/normal.h>
#include <numcpp/core/utils.h>

using namespace blitz;

size_t performance01Blitz(size_t N, size_t M)
{

  auto t = numcpp::tic();

  Array<double,1> linspace(N);

  linspace = 56;


  for(int l=0;l<10;l++)
  {
    Array<std::complex<double>,1> x(N);
    x = 0;
    for(auto i=0; i<M; i++)
      x += 1;

    for(auto i=0; i<N; i++)
      x(i) += i;

    for(auto i=0; i<M; i++)
      x += ((double)i)*exp(2*M_PI*i*linspace  * std::complex<double>(0,1));

    for(auto i=0; i<M; i++)
      x += i;
  }

  return numcpp::toc(t, false);
}

size_t performance02Blitz(size_t N, size_t M)
{
    Array<float,1> x(N);
    Array<float,2> A(N,N);
    Array<float,1> y(N);

    x =1; A=2;

    firstIndex k;
    secondIndex j;

    auto t = numcpp::tic();

    for(auto i=0; i<M; i++)
    {
      y += sum(A(j,k)*x(k),k) + sum(A(j,k)*x(k),k)*(1.0/(i+1));
    }

    return numcpp::toc(t, false);
}


size_t performance03Blitz(size_t N, size_t M)
{
    Array<float,2> B(N,N);
    Array<float,2> A(N,N);
    Array<float,2> C(N,N);

    B=1; A=2;

    firstIndex k;
    secondIndex j;
    thirdIndex l;

    auto t = numcpp::tic();

    for(auto i=0; i<M; i++)
    {
       C += sum(A(j,k)*B(k,j),k)*i;
    }

    return numcpp::toc(t, false);
}

int main(int argc, char* argv[])
{
  int l = 6;
  int code = 0;
  if(argc > 1)
    l = atoi(argv[1]);
  if(argc > 2)
    code = atoi(argv[2]);

  auto N = pow(2,l);
  auto M = N;
  size_t time;


  switch (code) {
  case 1:
      time = performance01Blitz(N,10);
      break;
  case 2:
      time = performance02Blitz(N,M);
      break;
  case 3:
      time = performance03Blitz(N,M);
      break;
  }

  std::cout << time << std::endl;

  return 0;
}
