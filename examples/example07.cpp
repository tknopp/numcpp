#include <numcpp.h>
using namespace numcpp;

int main()
{
  size_t M = 3, N = 3;
  auto A = array<double>({1, 7, 3,
                          9, 9, 4,
                          1, 0, 4}, M, N);


  auto x = eval(ones(N));
  auto y = dot(A,x);

  print(A);
  print(x);
  print(y);

  auto z = solve(A, y);

  print(z);
}
/*!
@page example05 Solving Linear Systems

@code
int main()
{
  size_t M = 3, N = 3;
  auto A = array<double>({1, 7, 3,
                          9, 9, 4,
                          1, 0, 4}, M, N);


  auto x = eval(ones(N));
  auto y = dot(A,x);

  auto z = solve(A, y);

}
@endcode
*/

