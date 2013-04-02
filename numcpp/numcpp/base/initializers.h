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

template<class T, int D>
class ConstantArray : public AbstractArray<T,D,ConstantArray<T,D> >
{
public:
  typedef int value_type;
  static const int dim = D;

  template<class Int=size_t>
  ConstantArray(T value, std::array<Int,D> shape)
   : shape_(shape)
   , value_(value)
  {
  }

  /*template<class...A>
  ConstantArray(T value, A...args)
   : shape_({args...})
   , value_(value)
  {
  }*/

  size_t size() const
  {
    return prod(shape_);
  }

  const std::array<size_t,D>& shape() const
  {
    return shape_;
  }

  value_type operator[](size_t /* index */) const
  {
    return value_;
  }

  template<class...A>
  typename std::enable_if< !isSlicedArray< A... >::value,
        T &
        >::type
  operator()(A...args) const
  {
    return value_;
  }

protected:
  std::array<size_t,D> shape_;
  T value_;
};

template<class T>
class LinearVector : public AbstractVector<T,LinearVector<T> >
{
public:
  typedef T value_type;
  static const int dim = 1;

  LinearVector(T start, T end, T step)
   : start_(start)
   , end_(end)
   , step_(step)
  {
    shape_[0] = round((end-start) / step );
  }

  size_t size() const
  {
    return shape_[0];
  }

  const std::array<size_t,1>& shape() const
  {
    return shape_;
  }

  value_type operator[](size_t index) const
  {
    return operator()(index);
  }

  value_type operator()(size_t index) const
  {
    return start_ + index*step_;
  }

protected:
  std::array<size_t,1> shape_;
  T start_;
  T end_;
  T step_;
};


// zeros
template<int D, class Int=size_t>
ConstantArray<int,D> zeros(std::array<Int,D> shape)
{
  return ConstantArray<int,D> (0, shape);
}

template<class...A>
ConstantArray<int,sizeof...(A)> zeros(A...args)
{
  return ConstantArray<int,sizeof...(A)> (0, {((size_t)args)...});
}

// ones

template<int D, class Int=size_t>
ConstantArray<int,D> ones(std::array<Int,D> shape)
{
  return ConstantArray<int,D> (1, shape);
}

template<class...A>
ConstantArray<int,sizeof...(A)> ones(A...args)
{
  return ConstantArray<int,sizeof...(A)> (1, {((size_t)args)...});
}

// linspace

inline LinearVector<double> linspace(double start, double end, size_t size)
{
  return LinearVector<double>(start, end, (end-start) / size);
}

inline LinearVector<int> range(size_t start, size_t end, size_t step=1)
{
  return LinearVector<int>(start, end, step);
}


template<class T, class R1, class U, class R2>
std::pair< Matrix<T>, Matrix<U> >
meshgrid(const AbstractVector<T,R1>& x, const AbstractVector<U,R2>& y)
{
  auto N = shape(x,0);
  auto M = shape(y,0);
  Matrix<T> xx(M,N), yy(M,N);

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


inline Matrix<double> phantom(size_t N)
{
    // phantom(N)
    //  generates the (modified) Shepp-Logan phantom of P. Toft
    //  as an NxN matrix.
    //
    // Reference: Peter Toft: "The Radon Transform - Theory and Implementation", Ph.D. thesis.
    //   Department of Mathematical Modelling, Technical University of Denmark, June 1996. 326 pages.

    // Author: Markus Fenn, 2005

    Matrix<double> I = zeros(N,N);

    auto z = meshgrid( linspace(-1,1,N) , (-1)*linspace(-1,1,N) );
    auto x = z.first;
    auto y = z.second;

    //auto s = 0.8;
    auto s = 0.4;

    I += 1.0 * ( pow(x/0.69,2.0)+pow(y/0.92,2.0) < 1);
    I += - s * ( pow(x/0.6624,2)+pow((y+0.0184)/0.874,2) < 1 );
    I += - 0.2 * ( pow( (cos(-18.0/360*2*pi)*(x-0.22)+sin(-18.0/360*2*pi)*y)/0.11,2) +
        pow( (sin(-18.0/360*2*pi)*(x-0.22)-cos(-18.0/360*2*pi)*y)/0.31,2) < 1 );
    I += - 0.2 * ( pow( (cos( 18.0/360*2*pi)*(x +0.22)+sin( 18.0/360*2*pi)*y)/0.16,2) +
        pow( (sin( 18.0/360*2*pi)*(x +0.22)-cos( 18.0/360*2*pi)*y)/0.41, 2) <1 );
    I += 0.1 * ( pow(x/0.21,2)+pow((y-0.35)/0.25,2) <1);
    I += 0.1 * ( pow(x/0.046,2)+pow((y-0.1)/0.046,2) <1 );
    I += 0.1 * ( pow(x/0.046,2)+pow((y+0.1)/0.046,2) <1 );
    I += 0.1 * ( pow((x+0.08)/0.046,2)+pow((y+0.605)/0.023,2) <1 );
    I += 0.1 * ( pow(x/0.023,2)+pow((y+0.606)/0.023,2) <1 );
    I += 0.1 * ( pow((x-0.06)/0.023,2)+pow((y+0.605)/0.046,2) <1 );

    return I;
}

/*!
Create a vector of type T that ...

bbb
*/
template<class T=double>
Vector<T> array(std::vector<T> x)
{
  Vector<T> y(x.size());

  for(size_t i=0; i<x.size(); i++)
    y[i] = x[i];

  return y;
}

template<class T=double, class...A>
Array<T, sizeof...(A)> array(std::vector<T> x, A...shape)
{
  Array<T, sizeof...(shape)> y(((size_t)shape)...);

  for(size_t i=0; i<x.size(); i++)
    y[i] = x[i];

  return y;
}

/*! @} */

}

#endif
