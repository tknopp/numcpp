#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

size_t performance01Eigen(size_t N, size_t M)
{

  auto t = tic();


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

  return toc(t,false);

}

size_t performance02Eigen(size_t N, size_t M)
{
    Eigen::VectorXf x = Eigen::VectorXf::Random(N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::VectorXf y(N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      y += A*x*i  + A*x*(1.0/(i+1)); //; + A*Eigen::VectorXf::Ones(N)*i;
    }

    return toc(t,false);

}

size_t performance03Eigen(size_t N, size_t M)
{
    Eigen::MatrixXf B = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf C(N,N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      C += A*B*i; //; + A*Eigen::VectorXf::Ones(N)*i;
    }

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
      time = performance01Eigen(N,10);
      break;
  case 2:
      time = performance02Eigen(N,M);
      break;
  case 3:
      time = performance03Eigen(N,M);
      break;
  }

  std::cout << time << std::endl;

  return 0;
}
