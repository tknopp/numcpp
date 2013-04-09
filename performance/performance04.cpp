#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

size_t performance04NumCpp(size_t N, size_t M)
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

    return toc(t);
}

size_t performance04Eigen(size_t N, size_t M)
{
    Eigen::MatrixXf B = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::MatrixXf C(N,N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      C += A*B*i; //; + A*Eigen::VectorXf::Ones(N)*i;
    }

   return toc(t);

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
  return 77;

  if(code == 0)
  {
    std::cout << "NumCpp\n";
    return performance04NumCpp(N,M);
  } else
  {
    std::cout << "Eigen\n";
    return performance04Eigen(N,M);
  }

  return 0;
}
