#include <numcpp.h>
using namespace numcpp;

int main()
{
  size_t N = 256;

  auto x = randn<double>(N,N);
  auto file = h5create("test.h5");
  h5write(x, file, "/testData");
  h5close(file);

  //auto B = h5read<T,2>("test.h5", "/testData");


}


