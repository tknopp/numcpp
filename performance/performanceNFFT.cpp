#define NUMCPP_OUTPUT_TIMINGS
#include <numcpp.h>
using namespace numcpp;

int main()
{
  size_t m = 4;
  double sigma = 2.0;

  {
    size_t N = pow(2,19);
    size_t M = N;

    Array<double> x = rand<double>(M) - 0.5;
    Array<cdouble> fHat = rand<double>(M);

    std::cout << "NFFT Performance Test 1D:" << std::endl;

    auto t=tic();
    NFFTPlan<double> plan(x,{N},m,sigma);
    std::cout << "initialization: ";
    toc(t);

    t=tic();
    auto fApprox = plan.adjoint(fHat);
    std::cout << "adjoint: ";
    toc(t);

    t=tic();
    auto fHat2 = plan.trafo(fApprox);
    std::cout << "trafo: ";
    toc(t);
  }

  {
    size_t N = 1024;
    size_t M = N*N;

    std::cout << "NFFT Performance Test 2D:" << std::endl;

    Array<double> x = rand<double>(M,2) - 0.5;
    Array<cdouble> fHat = rand<double>(M);

    auto t=tic();
    NFFTPlan<double> plan(x,{N,N},m,sigma);
    std::cout << "initialization: ";
    toc(t);

    t=tic();
    auto fApprox = plan.adjoint(fHat);
    std::cout << "adjoint: ";
    toc(t);

    t=tic();
    auto fHat2 = plan.trafo(fApprox);
    std::cout << "trafo: ";
    toc(t);
  }

}

