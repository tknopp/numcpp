#ifndef NUMCPP_SPARSEVECTOR_H
#define NUMCPP_SPARSEVECTOR_H

#include "../core.h"

namespace numcpp
{

template<class T>
class SparseVector : public AbstractArray<T,1,SparseVector<T>>
{
public:

  SparseVector(const SparseVector& rhs)
    : shape_(rhs.shape_)
    , data_(rhs.data_)
    , index_(rhs.index_)
  {
  }

  SparseVector(const Vector<T>& data, const Vector<size_t>& index, size_t N)
    : shape_({N})
    , data_(data)
    , index_(index)
  {
  }

  const std::array<size_t,1>& shape() const
  {
    return shape_;
  }

  size_t index(size_t n) const
  {
    return index_(n);
  }

  T& data(size_t n)
  {
    return data_(n);
  }

  T data(size_t n) const
  {
    return data_(n);
  }

  Vector<T>& data()
  {
    return data_;
  }

  Vector<T> data() const
  {
    return data_;
  }

private:
  std::array<size_t,1> shape_;
  Vector<T> data_;
  Vector<size_t> index_;
};


template<class T, class U>
  COMMON_TYPE(T,U)
  dot(const SparseVector<T>& x, const SparseVector<U>& y)
{
  //return sum(x*y);
  return 0; //TODO
}

template<class T, class U, class V>
  COMMON_TYPE(T,U)
  dot(const SparseVector<T>& x, const AbstractVector<U,V>& y)
{
  T res = 0;
  for(size_t n=0; n<x.data().size(); n++)
  {
    res += x.data(n) * y(x.index(n));
  }
  return res;
}

template<class T, class U, class R>
  COMMON_TYPE(T,U)
  dot(const AbstractVector<T,R>& x, const SparseVector<U>& y)
{
  return dot(y,x);
}

template<class T>
double norm(const SparseVector<T>& x, double p=2.0)
{
  if(p == 0)
    return ( sum( abs( x.data() ) != 0 ) );
  if(std::isinf(p))
    return ( max( abs( x.data() ) ) );

  return std::pow( sum( pow( abs( x.data() ), p) ), 1./p );
}

}

#endif
