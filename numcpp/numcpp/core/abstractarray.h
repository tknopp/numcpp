#ifndef NUMCPP_ABSTRACTARRAY_H
#define NUMCPP_ABSTRACTARRAY_H

#include "slice.h"
#include <array>

/// NumCpp Namspace
namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

template<class T, int D>
class StridedArray;

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

  T& operator[](size_t index)
  {
    return getSelf()[index];
  }

  T operator[](size_t index) const
  {
    return getSelf()[index];
  }

  template<class...A>
  typename std::enable_if< isElementAccess< A... >::value,
        T
        >::type
  operator()(A...args) const
  {
    return getSelf()(args...);
  }

  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isSlicedArray< A... >::value ,
        StridedArray<T,countSlices< A... >::value>
        >::type
  {
    return getSelf()(args...);
  }

protected:
  Derived & getSelf() { return *static_cast<Derived *>(this); }
  Derived const & getSelf() const { return *static_cast<Derived const *>(this); }

};

template <class T, class Derived>
using AbstractVector = AbstractArray<T, 1, Derived >;

template <class T, class Derived>
using AbstractMatrix = AbstractArray<T, 2, Derived >;




/// Abstract strided array
template<class T, int D, class Derived>
class AbstractStridedArray : public AbstractArray<T,D,AbstractStridedArray<T,D,Derived>>
{
public:
  typedef T value_type;
  static const int dim = D;

  size_t ndims() const
  {
    return D;
  }

  /*size_t size() const
  {
    return getSelf().size();
  }*/

  size_t size() const
  {
      size_t size = shape()[0];
      for(int d=1; d<D; d++)
          size *= shape()[d];
      return size;
  }

  const std::array<size_t,D>& shape() const
  {
    return getSelf().shape();
  }

  size_t shape(int d) const
  {
    return shape()[d];
  }

  const std::array<size_t,D>& strides() const
  {
    return getSelf().strides();
  }

  T& operator[](size_t index)
  {
    return getSelf()[index];
  }

  T operator[](size_t index) const
  {
    return getSelf()[index];
  }

  T& operator[](std::array<size_t,D> index)
  {
    return data()[flatIndex(index)];
  }

  T operator[](std::array<size_t,D> index) const
  {
    return data()[flatIndex(index)];
  }


  /*template<class...A>
  typename std::enable_if< !isSlicedArray< A... >::value,
        T &
        >::type
  operator()(A...args) const
  {
    return getSelf()(args...);
  }*/

  template<class...A>
  typename std::enable_if< isElementAccess< A... >::value,
        T &
        >::type
  operator()(A...args) const
  {
    std::array< size_t , D> index({((size_t)args)...});
    return data()[flatIndex(index)];
  }

  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isSlicedArray< A... >::value ,
        StridedArray<T,countSlices< A... >::value>
        >::type
  {
    return getSelf()(args...);
  }

  T* data() const
  {
    return getSelf().data();
  }

  const MemoryBlock& getMem() const
  {
    return getSelf().getMem();
  }

  // Assignment Operators

  template <class U, class V>
  AbstractStridedArray& operator+= (const AbstractArray<U,D,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) += rhs[i];

    return *this;
  }

  template <class U, class V>
  AbstractStridedArray& operator= (const AbstractArray<U,D,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
  }


  AbstractStridedArray& operator*= (const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) *= rhs;

    return *this;
  }

  AbstractStridedArray& operator=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) = rhs;

    return *this;
  }

  AbstractStridedArray& operator+=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) += rhs;

    return *this;
  }

protected:
  template<class Int>
  size_t flatIndex(const std::array<Int,D>& index) const
  {
    return getSelf().flatIndex(index);
  }

  Derived & getSelf() { return *static_cast<Derived *>(this); }
  Derived const & getSelf() const { return *static_cast<Derived const *>(this); }

};

template <class T, class Derived>
using AbstractStridedVector = AbstractStridedArray<T, 1, Derived >;

template <class T, class Derived>
using AbstractStridedMatrix = AbstractStridedArray<T, 2, Derived >;

/*! @} */

}

#endif
