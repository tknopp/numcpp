#include <numcpp.h>
#include <numcpp/solvers.h>
using namespace numcpp;

int main()
{
  size_t n = 256;
  size_t N = n*n;
  size_t M = N / 4;

  auto a = array<cdouble>({1,2,3,4});
  auto idx = array<size_t>({0,1,2,3});

  SparseFFTArray<cdouble> F2(idx,4);

  print(dot(F2,a));
  print(solve(F2,a));

  //Array<cdouble> p = reshape(phantom(n),N);
  Array<cdouble> p = zeros(N);
  p(n*n/2 + n/4) = 1;

  auto tmp2 = eval(abs(reshape(p,n,n)));
  export_image( tmp2, "CSIm.png");

  //auto indices = eval(range(0,N-1));
  auto indices = randi<size_t>(N-1, M);

  SparseFFTArray<cdouble> F(indices,n,n);

  auto y = dot(F,p);
  auto noise = randn<double>(M);
  noise *= max(abs(y))*0.5;
  y += noise;

  {
    auto z = solve(F, y);
    export_image( eval( abs(reshape(z,n,n) ) ), "CSNaiv.png");
    std::cout << "error naiv: " << nrmsd(p, z) << " max " <<  max(abs(z)) << std::endl;
    std::cout << z.getMem() <<std::endl;
  }

  {
    auto z = SL0(F, y, 30, 3, 2);
    export_image( eval( abs(reshape(z,n,n) )), "CSSL0.png");
    std::cout << "error SL0: " << nrmsd(p, z) << " max " <<  max(abs(z)) << std::endl;
    std::cout << z.getMem() <<std::endl;
  }

  {
    auto z = fista(F, y, 30, 3, 1e-8);
    export_image( eval ( abs(reshape(z,n,n) )), "CSFista.png");
    std::cout << "error Fista: " << nrmsd(p, z) << " max " <<  max(abs(z)) << std::endl;
    std::cout << z.getMem() <<std::endl;
  }

}

/*!
@page example07 Compressed Sensing Example

@code
int main()
{
  size_t N = 256;

  auto p = phantom(N);
  export_image(p, "phantom.png");

  auto y = dwt(p, wavelets::Haar);
  export_image(y, "waveletHaar.png");

  auto y = dwt(p, wavelets::Daubechies4);
  export_image(y, "waveletDaubechies4.png");

  auto z = idwt(y, wavelets::Daubechies4);
  export_image(z, "reco.png");
}
@endcode
*/

