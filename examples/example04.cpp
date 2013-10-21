#include <numcpp.h>
using namespace numcpp;

int main()
{
  auto x = array({1,2,3,
                  4,5,6,
                  7,8,9}, 3, 3);

  auto y = x;                             // create a view of x

  auto z = copy(x);                       // create a deep copy of x

  auto x1 = x(2,2);                       // access element at index 2,2
  print(x1);                              // x1 = 9

  auto x2 = x( S{0,2}, S{0,2} );          // select block within x
  print(x2);                              // x2 = [1 2; 4 5]

  auto x3 = x( S{0,-1,2}, S{0,-1,2} );  // select strided block within x
  print(x3);                              // x3 = [1 3; 7 9]

  auto x4 = x( full, 0 );                 // select first column of x
  print(x4);                              // x4 = [1 4 7]

  x(S{0,-1,2}, full) = 1;                // set first and last row to one
  print(x);                               // [1 1 1; 4 5 6; 1 1 1]

  x(1, full) = z(0, full);                // copy first row of z into the second row of x
  print(x);                               // [1 1 1; 1 2 3; 1 1 1]

  auto u = eval(range(0,10));             // create array u = [0 ... 9]
  //auto u1 = u(V{0, 1, 7, 5});             // non-regular indexing of u
  //print(u1);                              // u1 = [0 1 6 4]
}

/*!
@page example04 Array Slicing
This is a simple example showing the indexing an slicing capabilities of NumCpp.

@code
#include <numcpp.h>
using namespace numcpp;

int main()
{
  auto x = array({1,2,3,
                  4,5,6,
                  7,8,9}, 3, 3);

  auto y = x;                             // create a view of x

  auto z = copy(x);                       // create a deep copy of x

  auto x1 = x(2,2);                       // access element at index 2,2
  print(x1);                              // x1 = 9

  auto x2 = x( S{0,2}, S{0,2} );          // select block within x
  print(x2);                              // x2 = [1 2; 4 5]

  auto x3 = x( S{0,end,2}, S{0,end,2} );  // select strided block within x
  print(x3);                              // x3 = [1 3; 7 9]

  auto x4 = x( full, 0 );                 // select first column of x
  print(x4);                              // x4 = [1 4 7]

  x(S{0,end,2}, full) = 1;                // set first and last row to one
  print(x);                               // [1 1 1; 4 5 6; 1 1 1]

  x(1, full) = z(0, full);                // copy first row of z into the second row of x
  print(x);                               // [1 1 1; 1 2 3; 1 1 1]

  auto u = eval(range(0,10));             // create array u = [0 ... 9]
  auto u1 = u(V{0, 1, 7, 5});             // non-regular indexing of u
  print(u1);                              // u1 = [0 1 6 4]
}

@endcode
*/

