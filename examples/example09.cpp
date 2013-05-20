#include <numcpp.h>
using namespace numcpp;

int main()
{
 auto x = array({1,2,3,4});

 tofile(x, "test.txt", " ");
}
