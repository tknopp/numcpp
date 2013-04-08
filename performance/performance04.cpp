#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

void performance04NumCpp(size_t N, size_t M)
{
    Matrix<float> B = randn<float>(N,N);
    Matrix<float> A = randn<float>(N,N);
    Matrix<float> C(N,N);


    auto t = tic();

    //EnableCaching();

    for(auto i=0; i<M; i++)
    {
       dot(A,B); // + dot(A,ones(N))*i;
    }

    //DisableCaching();

    toc(t);
}

void performance04Eigen(size_t N, size_t M)
{
    Eigen::MatrixXf B = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf C(N,N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      C += A*B*i; //; + A*Eigen::VectorXf::Ones(N)*i;
    }

    toc(t);

}

int main()
{
  for(int l=3; l<10; l++)
  {
    auto N = pow(2,l);
    auto M = N;
    std::cout << "NumCpp\n";
    performance04NumCpp(N,M);
    std::cout << "Eigen\n";
    performance04Eigen(N,M);
    std::cout << "\n";
  }

  return 0;
}
