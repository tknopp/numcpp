#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

size_t performance01Numcpp(size_t N, size_t M)
{

  auto t = tic();

  for(int l=0;l<10;l++)
  {
    Array<cdouble> x = zeros(N);
    for(auto i=0; i<M; i++)
      x += ones(N);

    for(auto i=0; i<N; i++)
      x(i) += i;

    for(auto i=0; i<M; i++)
      x += i*exp(2*pi*I*i*linspace(0,1,N));

    for(auto i=0; i<M; i++)
      x += i;
  }

  return toc(t, false);
}

size_t performance02Numcpp(size_t N, size_t M)
{
    Array<float> x = randn<float>(N);
    Array<float> A = randn<float>(N,N);
    Array<float> y(N);


    auto t = tic();

    //EnableCaching();

    for(auto i=0; i<M; i++)
    {
      y += dot(A,x)*i + dot(A,x)*(1.0/(i+1)); // + dot(A,ones(N))*i;
    }

    //DisableCaching();

    return toc(t, false);
}


size_t performance03Numcpp(size_t N, size_t M)
{
    Array<float> B = randn<float>(N,N);
    Array<float> A = randn<float>(N,N);
    Array<float> C(N,N);


    auto t = tic();

    //EnableCaching();

    for(auto i=0; i<M; i++)
    {
       C += dot(A,B); // + dot(A,ones(N))*i;
    }

    //DisableCaching();

    return toc(t, false);
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
      time = performance01Numcpp(N,10);
      break;
  case 2:
      time = performance02Numcpp(N,M);
      break;
  case 3:
      time = performance03Numcpp(N,M);
      break;
  }

  std::cout << time << std::endl;

  return 0;
}
