#include <numcpp.h>
using namespace numcpp;

int main()
{
  // create Shepp Logan phantom (type Matrix<double>)
  auto x = phantom(256);

  // calculate the absolute value of the FFT of x
  // the eval call is necessary as abs is an expression template
  auto y = eval(abs(fftshift(fft(x))));

  // export x and y to an pdf file using different colormaps
  export_pdf( x, "shepp_logan_phantom.pdf", colormaps::autumn);
  export_pdf( y, "shepp_logan_phantom_fft.pdf");

  // export x and y into the same hdf5 file
  hdf5write( x, "shepp_logan_phantom.h5", "/shepp_logan" );
  hdf5write( y, "shepp_logan_phantom.h5", "/shepp_logan_fft" );

  // export x into a simple binary file
  tofile( x, "shepp_logan_phantom.dat");
}

