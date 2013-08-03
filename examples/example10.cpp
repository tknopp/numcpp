#include <numcpp.h>
using namespace numcpp;

template<class T>
void sum(const DynDimArray<T>& x) {}

int main()
{
  Array<double,2> x(2,2);
  DynDimArray<double> y(2,2);

  x(0,0) = 0;
  x(0,1) = -1;
  x(1,0) = 2;
  x(1,1) = 3;

  std::cout << x;

  y = abs(x) + pow(x,2);

  sum<double>(abs(y));

  std::cout << y;

}
