#ifndef NUMCPP_INITIALIZERS_H
#define NUMCPP_INITIALIZERS_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup initializers
@brief Initializier array functions
@ingroup base
@{
*/

template<class T>
class ConstantArray : public AbstractArrayExpression<T,ConstantArray<T> >
{
public:

  template<class Int=size_t>
  ConstantArray(T value, const std::vector<Int>& shape)
   : shape_(shape)
   , value_(value)
  {
  }

  size_t size() const
  {
    return prod(shape_);
  }

  const std::vector<size_t>& shape() const
  {
    return shape_;
  }

  T operator[](size_t /* index */) const
  {
    return value_;
  }

protected:
  std::vector<size_t> shape_;
  T value_;
};

template<class T>
class LinearArray : public AbstractArrayExpression<T,LinearArray<T> >
{
public:
  LinearArray(T start, T end, T step)
   : start_(start)
   , end_(end)
   , step_(step)
   , shape_(1)
  {
    shape_[0] = round((end-start) / step );
  }

  size_t size() const
  {
    return shape_[0];
  }

  const std::vector<size_t>& shape() const
  {
    return shape_;
  }

  T operator[](size_t index) const
  {
    return start_ + index*step_;
  }

protected:
  std::vector<size_t> shape_;
  T start_;
  T end_;
  T step_;
};


/// Return an constant array containing only zero values.
/// Note that this function returns an expression template.
inline ConstantArray<int> zeros(const std::vector<size_t>& shape)
{
  return ConstantArray<int> (0, shape);
}

/// \overload
template<class...A>
ConstantArray<int> zeros(A...args)
{
	std::vector<size_t> shape = { ((size_t)args)... };
	return ConstantArray<int>(0, shape);
}

/// Return an constant array containing only ones as values.
/// Note that this function returns an expression template.
template<class Int=size_t>
ConstantArray<int> ones(std::vector<Int> shape)
{
  return ConstantArray<int> (1, shape);
}

/// \overload
template<class...A>
ConstantArray<int> ones(A...args)
{
	std::vector<size_t> shape = { ((size_t)args)... };
	return ConstantArray<int>(1, shape);
}

/// Return a vector containing linear spaced values.
inline LinearArray<double> linspace(double start, double end, size_t size)
{
  return LinearArray<double>(start, end + (end-start) / (size-1), (end-start) / (size-1));
}

/// Return a vector containing linear spaced values.
inline LinearArray<int> range(size_t start, size_t end, size_t step=1)
{
  return LinearArray<int>(start, end, step);
}

/*!
Return coordinate matrices from two coordinate vectors.

Example:
\code
auto x = range(0,3);       // x = [0, 1, 2]
auto cc = meshgrid(x,x);
auto xx = cc.first;        // xx = [0, 1, 2; 0, 1, 2; 0, 1, 2]
auto yy = cc.second;       // yy = [0, 0, 0; 1, 1, 1; 2, 2, 2]
\endcode
*/
template<class T, class U>
std::pair< Array<U>, Array<T> >
meshgrid(const Array<U>& x, const Array<T>& y)
{
  auto N = shape(x,0);
  auto M = shape(y,0);
  Array<U> xx(M,N);
  Array<T> yy(M,N);

  for(size_t n=0; n<N; n++)
  {
    yy(full,n) = y;
  }

  for(size_t m=0; m<M; m++)
  {
    xx(m,full) = x;
  }

  return std::make_pair(xx, yy);
}

/*!
Generates the (modified) Shepp-Logan phantom of P. Toft as an NxN matrix.

Reference: Peter Toft: "The Radon Transform - Theory and Implementation", Ph.D. thesis.
   Department of Mathematical Modelling, Technical University of Denmark, June 1996. 326 pages.
*/
Array<double> phantom(size_t N);

/*!
The array function creates an Array object from an std::vector and in turn
an std::initializer list. For multidimensional arrays, the shape has to be
specified. For 1D arrays, the shape parameter can be omitted.

Example:
\code
auto x = array({1,2,3});         // x = [1, 1, 3]      Type: Array<double,1>
auto y = array({1,2,3,4},2,2);   // y = [1, 2; 3, 4]   Type: Array<double,2>
auto z = array<int>({1,2,3});    // z = [1, 2, 3]      Type: Array<int,1>
\endcode
*/
template<class T=double, class...A>
Array<T> array(std::vector<T> x, A...shape)
{
  Array<T> y(((size_t)shape)...);

  std::copy(x.begin(), x.end(), y.data());

  return y;
}

template<class T=double>
Array<T> array(std::vector<T> x)
{
  Array<T> y(x.size());

  for(size_t i=0; i<x.size(); i++)
    y[i] = x[i];

  return y;
}

/*! @} */

}

#endif
