#include <numcpp.h>
using namespace numcpp;

int main()
{
  size_t n = 256;
  size_t N = n*n;
  size_t M = N / 4;

  Vector<cdouble> p = reshape(phantom(n),N);
  auto indices = rand<size_t>(N, M);

  SparseFFTMatrix<cdouble,2> F(indices,n,n);

  auto y = dot(F,p);

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

