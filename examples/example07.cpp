#include <numcpp.h>
using namespace numcpp;

int main()
{
  size_t N = 8;
  size_t M = N;

  Vector<double> x = linspace(-0.5, 0.5, M);//rand(M) - 0.5

  Vector<cdouble> fHat = range(0,M); //randn<double>(M); + I*randn<double>(M);//+im*randn(M)

  auto f = ndftAdjoint(fHat, x);
  auto fApprox = nfftAdjoint(fHat, x);

  print(f);
  print(fApprox);

  //f = ndft_adjoint(F,fHat)
  //fApprox = nfft_adjoint(F,fHat)
  //println( norm(f-fApprox) / norm(f)  )

  //println(F)
}

