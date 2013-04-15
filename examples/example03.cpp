#include <numcpp.h>
using namespace numcpp;

int main()
{
  // create Shepp Logan phantom (type Matrix<double>)
  auto x = phantom(256);

  // calculate the absolute value of the FFT of x
  // the eval call is necessary as abs is an expression template
  auto y = eval(log(abs(fftshift(fft(x)))));

  // export x and y to image files (pdf/png) using different colormaps
  export_image( x, "shepp_logan_phantom.pdf", colormaps::winter);
  export_image( y, "shepp_logan_phantom_fft.png");

  // export x and y into the same hdf5 file
  auto file = h5create("shepp_logan_phantom.h5");
  h5write( x, file, "/shepp_logan" );
  h5write( y, file, "/shepp_logan_fft" );
  h5close(file);

  // export x into a simple binary file
  tofile( x, "shepp_logan_phantom.dat");
}

/*!
@page example03 Data and Image Export Example
Example showing the data and image export capabilities of NumCpp.

NumCpp uses the Cairo graphics library for exporting arrays as images.

@code
#include <numcpp.h>
using namespace numcpp;

int main()
{
  // create Shepp Logan phantom (type Matrix<double>)
  auto x = phantom(256);

  // calculate the absolute value of the FFT of x
  // the eval call is necessary as abs is an expression template
  auto y = eval(log(abs(fftshift(fft(x)))));

  // export x and y to image files (pdf/png) using different colormaps
  export_image( x, "shepp_logan_phantom.pdf", colormaps::winter);
  export_image( y, "shepp_logan_phantom_fft.png");

  // export x and y into the same hdf5 file
  auto file = h5create("shepp_logan_phantom.h5");
  h5write( x, file, "/shepp_logan" );
  h5write( y, file, "/shepp_logan_fft" );
  h5close(file);

  // export x into a simple binary file
  tofile( x, "shepp_logan_phantom.dat");
}
@endcode

The generate PDF file contain the following images:

@image html exampleExport.png

*/

