#include <numcpp.h>
using namespace numcpp;

int main()
{
  auto x = array({1,2,3,
                  4,5,6,
                  7,8,9}, 3, 3);

  auto y = x;                     // create a view of x

  auto z = copy(x);               // create a deep copy of x

  x(slice(0,end,2), full) = 1;    // set first and last row to one
  print(x);                       // [1 1 1; 4 5 6; 1 1 1]

  x(1, full) = z(0, full);        // copy first row of z into the second row of x
  print(x);                       // [1 1 1; 1 2 3; 1 1 1]
}

/*!
@page example04 Array Slicing
This is a simple example showing the slicing capabilities of NumCpp.

@code
int main()
{
  auto x = array({1,2,3,
                  4,5,6,
                  7,8,9}, 3, 3);

  auto y = x;                     // create a view of x

  auto z = copy(x);               // create a deep copy of x

  x(slice(0,end,2), full) = 1;    // set first and last row to one
  print(x);                       // [1 1 1; 4 5 6; 1 1 1]

  x(1, full) = z(0, full);        // copy first row of z into the second row of x
  print(x);                       // [1 1 1; 1 2 3; 1 1 1]
}
@endcode
*/

