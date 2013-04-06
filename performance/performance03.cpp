#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

void performance03NumCpp(size_t N, size_t M)
{
    Vector<float> x = randn<float>(N);
    Matrix<float> A = randn<float>(N,N);
    Vector<float> y(N);


    auto t = tic();

    //EnableCaching();

    for(auto i=0; i<M; i++)
    {
      y += dot(A,x) + dot(A,ones(N))*i;
    }

    //DisableCaching();

    toc(t);
}

void performance03Eigen(size_t N, size_t M)
{
    Eigen::VectorXf x = Eigen::VectorXf::Random(N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::VectorXf y(N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      y += A*x + A*Eigen::VectorXf::Ones(N)*i;
    }

    toc(t);

}

int main()
{
  auto N = 1000;
  auto M = 1000;

  performance03NumCpp(N,M);
  performance03Eigen(N,M);

  return 0;
}
