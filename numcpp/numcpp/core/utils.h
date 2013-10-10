#ifndef NUMCPP_UTILS_H
#define NUMCPP_UTILS_H

#include <iostream>
#include <initializer_list>
#include <vector>
#include <chrono>

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

template<class Array>
typename Array::value_type prod(Array x)
{
  auto prod = x[0];

  for(size_t i=1; i<x.size(); i++)
    prod *= x[i];

  return prod;
}

/*template<class Array>
Array removeOnes(Array x)
{
  x.erase(std::remove(x.begin(), x.end(), 1), x.end());
  return x;
}*/

std::vector<size_t> multiIndex(const size_t& index, const std::vector<size_t>& shape);

size_t flatIndex(const std::vector<size_t>& index, const std::vector<size_t>& strides, size_t offset);

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

void copyShapeToSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis);

void copyShapeFromSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis);


template<class Array>
void println (const Array& x)
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
clock::time_point tic();

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
size_t toc(clock::time_point t, bool print=true);



/*! @} */


}

#endif
