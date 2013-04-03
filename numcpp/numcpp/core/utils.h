#ifndef NUMCPP_UTILS_H
#define NUMCPP_UTILS_H

#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <array>
#include <iterator>
#include <chrono>

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

template<class Vector>
typename Vector::value_type prod(Vector x)
{
  auto prod = x[0];

  for(size_t i=1; i<x.size(); i++)
    prod *= x[i];

  return prod;
}

template<class Vector>
Vector removeOnes(Vector x)
{
  x.erase(std::remove(x.begin(), x.end(), 1), x.end());
  return x;
}

template<class T>
void print(const T& x)
{
  std::cout << x << std::endl;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& x)
{
  for(auto& y: x)
    os << y << " ";
  return os;
}

template<class T, int D>
std::ostream& operator<< (std::ostream& os, const std::array<T,D>& x)
{
  std::copy(begin(x), end(x), std::ostream_iterator<T>(os, "\n"));
  //for(int i=0; i<2; i++)
  //  os << x[i] << " ";
  return os;
}


template<int D>
void copyShapeToSubArray(const std::array<size_t,D>& shapeIn, std::array<size_t,D-1>& shapeOut, int axis)
{
  int j=0;
  for(int i=0; i<D; i++)
    if(i != axis)
    {
      shapeOut[j] = shapeIn[i];
      j++;
    }
}

template<int D>
void copyShapeFromSubArray(const std::array<size_t,D-1>& shapeIn, std::array<size_t,D>& shapeOut, int axis)
{
  int j=0;
  for(int i=0; i<D; i++)
    if(i != axis)
    {
      shapeOut[i] = shapeIn[j];
      j++;
    }
}



template<class Vector>
void println (const Vector& x)
{
  for(int i=0; i<x.size(); i++)
    std::cout << x[i] << " ";
  std::cout << std::endl;
}

/// Clock type that can hold the current time point.
typedef std::chrono::high_resolution_clock clock;

/*!
Start a timer and return the current clock time.
Example:
@code
auto t = tic();
// do some calculations
toc(t);
@endcode
\sa toc()
*/
inline clock::time_point tic()
{
  return clock::now();
}

/*!
Returns the time that is ellapsed since the time point \a t and now.
If the parameter \a print is true, the function prints the ellapsed time
in a pretty format to the console.
@code
auto t = tic();
// do some calculations
toc(t);
@endcode
\sa tic()
*/
inline size_t toc(clock::time_point t, bool print=true)
{
  auto diff = tic() - t;
  if(print)
  {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(diff);
    if(hours.count() > 0)
      std::cout << hours.count() << "h";
    auto min = std::chrono::duration_cast<std::chrono::hours>(diff-hours);
    if(min.count() > 0)
      std::cout << min.count() << "m";
    auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff-hours-min);
    std::cout << sec.count() / 1000.0 << "s" << std::endl;
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
}



/*! @} */


}

#endif
