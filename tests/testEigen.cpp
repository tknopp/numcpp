#include <Eigen/Core>

using namespace Eigen;

int main()
{
    size_t N = 70;
    size_t M = 10;

    ArrayXcd x = ArrayXcd::Zero(N*N*N);
    ArrayXd y = ArrayXd::LinSpaced(N*N*N,0,1);

    for(int i=0; i<M; i++)
    {
        x += ArrayXcd::Ones(N*N*N);
    }

    for(int i=0; i<N; i++)
    {
        x(i) += i;
    }


    for(int i=0; i<M; i++)
    {
        x += i*exp(std::complex<double>(0,1)*ArrayXcd::Ones(N*N*N)*i*2);
    }

    //std::cout << x << std::endl;

   return 0;
}
