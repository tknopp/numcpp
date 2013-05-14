#include <numcpp.h>
using namespace numcpp;

int main()
{

  auto zz = randn<double>(4,4);
  print(zz);
  print(fftshift_(zz));


  size_t N = 16;
  size_t M = N;
  size_t m = 4;
  double sigma = 2.0;

  Vector<double> x = linspace(-0.5, 0.5, M);//rand(M) - 0.5
  Vector<cdouble> fHat = range(0,M); //randn<double>(M); + I*randn<double>(M);//+im*randn(M)

  auto f = ndftAdjoint(fHat, x, N);
  auto fApprox = nfftAdjoint(fHat, x, N, m, sigma);

  std::cout << "Error(nfft,adjoint,1D) = " << nrmsd(f, fApprox) << std::endl;

  auto f2 = ndft(fHat, x);
  auto f2Approx = nfft(fHat, x, m, sigma);

  std::cout << "Error(nfft,trafo,1D) = " << nrmsd(f2, f2Approx) << std::endl;


  {
    size_t N = 8;
    size_t M = N*N;

    Matrix<double> x = reshape(linspace(-0.5,0.5,2*M),M,2);
    Matrix<cdouble> f = reshape(range(0,N*N),N,N);

    auto fHat = ndft(f,x);
    auto fHatApprox = nfft(f,x,m,sigma);
    std::cout << "Error(nfft,trafo,2D) = " << nrmsd(fHat, fHatApprox) << std::endl;

    auto f2 = ndftAdjoint<2>(fHat,x,{N,N});
    auto fApprox2 = nfftAdjoint(fHat,x,{N,N},m,sigma);

    std::cout << "Error(nfft,adjoint,2D) = " << nrmsd(f2, fApprox2) << std::endl;
  }

  {
    size_t N = pow(2,19);
    size_t M = N;

    Vector<double> x = linspace(-0.5, 0.5, M);
    Vector<cdouble> fHat = range(0,M);

    std::cout << "Performance test 1D:" << std::endl;

    auto t=tic();
    auto fApprox = nfftAdjoint(fHat, x, N, m, sigma);
    toc(t);

    std::cout << "trafo:" << std::endl;

    t=tic();
    auto fApprox2 = nfft(fHat, x, m, sigma);
    toc(t);
  }

  {
    size_t N = 1024;
    size_t M = N*N;

    std::cout << "Performance test 2D:" << std::endl;

    Matrix<double> x = reshape(linspace(-0.5,0.5,2*M),M,2);
    Vector<cdouble> fHat = reshape(range(0,N*N),N*N);

    auto t=tic();
    auto fApprox2 = nfftAdjoint(fHat,x,{N,N},m,sigma);
    toc(t);

    std::cout << "trafo:" << std::endl;

    t=tic();
    auto fHatApprox2 = nfft(fApprox2, x, m, sigma);
    toc(t);
  }

}

