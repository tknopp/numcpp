#ifndef ZEROS_H
#define ZEROS_H

#include "../core.h"
#include <random>

namespace numcpp
{

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
auto zeros(A...args)  -> ConstantArray<int,sizeof...(A)>
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
auto ones(A...args) -> ConstantArray<int,sizeof...(A)>
{
  return ConstantArray<int,sizeof...(A)> (1, {((size_t)args)...});
}

// linspace

LinearVector<double> linspace(double start, double end, size_t size)
{
  return LinearVector<double>(start, end, (end-start) / size);
}

LinearVector<int> range(size_t start, size_t end, size_t step=1)
{
  return LinearVector<int>(start, end, step);
}


template<class T, class...A>
Array<T,sizeof...(A)> randn(A...args)
{
  Array<T,sizeof...(A)> x(args...);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<T> d(0,1);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}


template<class T, int D, size_t DNew>
Array<T,DNew> reshape(const Array<T,D>& x, const std::array<size_t,DNew>& shape)
{
  Array<T,DNew> y(x.getMem());
  std::copy(std::begin(shape), std::end(shape), begin(y.shape_));
  y.initContiguousStrides(0);

  return y;
}

template<class T, int D, class...A>
Array<T,sizeof...(A)> reshape(const Array<T,D>& x, A...args)
{
  std::array<size_t,sizeof...(A)> shape = {((size_t) args)...};
  return reshape(x, shape);
}

template<class T, int D, class R, class...A>
Array<T,sizeof...(A)> reshape(const AbstractArray<T,D,R>& x, A...args)
{
  Array<T,sizeof...(A)> y(args...);
  for(size_t i=0; i<y.size(); i++)
    y[i] = x[i];

  return y;
}


}

#endif // ZEROS_H
