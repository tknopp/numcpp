#ifndef NUMCPP_ABSTRACTARRAY_H
#define NUMCPP_ABSTRACTARRAY_H

#include "slice.h"
#include <array>

namespace numcpp
{

template<class T, int D>
class Array;

/**
 * Abstract Multidimensional Array
 *
 * AbstractArray
 *
 * @tparam T type of each array element
 * @tparam D dimension / rank of the array
 * @tparam Derived implementation of AbstractArray
 */
template<class T, int D, class Derived>
class AbstractArray
{
public:
  typedef T value_type;
  static const int dim = D;

  size_t ndims() const
  {
    return D;
  }

  size_t size() const
  {
    return getSelf().size();
  }

  const std::array<size_t,D>& shape() const
  {
    return getSelf().shape();
  }

  size_t shape(int d) const
  {
    return shape()[d];
  }

  value_type& operator[](size_t index)
  {
    return getSelf()[index];
  }

  value_type operator[](size_t index) const
  {
    return getSelf()[index];
  }

  template<class...A>
  typename std::enable_if< !isSlicedArray< A... >::value,
        T &
        >::type
  operator()(A...args) const
  {
    return getSelf()(args...);
  }

  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isSlicedArray< A... >::value ,
        Array<T,countSlices< A... >::value>
        >::type
  {
    return getSelf()(args...);
  }

  operator Array<T,D>()
  {
    Array<T,D> x(shape());
    return x;
  }

protected:
  Derived & getSelf() { return *static_cast<Derived *>(this); }
  Derived const & getSelf() const { return *static_cast<Derived const *>(this); }

};

template <class T, class Derived>
using AbstractVector = AbstractArray<T, 1, Derived >;

template <class T, class Derived>
using AbstractMatrix = AbstractArray<T, 2, Derived >;

template <class T, int D, class Derived>
int ndims(const AbstractArray<T,D,Derived>& x)
{
  return D;
}

template <class T, int D, class Derived>
size_t shape(const AbstractArray<T,D,Derived>& x, int d)
{
  return x.shape(d);
}


}

#endif
