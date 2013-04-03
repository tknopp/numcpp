#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include "memoryblock.h"
#include "utils.h"
#include "defines.h"
#include "slice.h"
#include "iterator.h"
#include "abstractarray.h"
#include "constants.h"

#include <initializer_list>
#include <iostream>
#include <complex>
#include <array>
#include <cmath>
#include <type_traits>
#include <utility>
#include <tuple>

#ifdef _OPENMP
# include <omp.h>
#endif

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

/*!
Multidimensional Strided Array

The core of the numcpp library is a multidimensional array object named Array.

@tparam T type of each array element
@tparam D dimension / rank of the array
*/
template<class T, int D>
class Array : public AbstractArray<T,D,Array<T,D> >
{
public:
  typedef T value_type;

  // Constructors

  Array()
  {
  }

  /**
   * Array Constructor
   *
   * Creates a numcpp array of dimension \b D and shape  \b shape
   *
   * @param shape D arguments describing the shape of the Array
   * @tparam Int integer type
   */
  template<class...A>
  explicit Array(A...shape)
    : shape_({((size_t)shape)...})
  {
    mem.allocate(prod(shape_)*sizeof(T));
    initContiguousStrides(0);
  }

  /**
   * Array Constructor
   *
   * Creates a numcpp array of dimension \b D and shape  \b shape
   *
   * @param shape std::array of type Int and length D describing the shape of the Array
   * @tparam Int integer type
   */
  template<class Int>
  Array(const std::array<Int,D>& shape)
    : mem(prod(shape)*sizeof(T))
  {
    std::copy(std::begin(shape),std::end(shape),std::begin(shape_));

    initContiguousStrides(0);
  }

  /**
   * Array Constructor
   *
   * Creates a numcpp array of dimension \b D and shape \b shape from exernal data
   *
   * @param data pointer to external data
   * @param ownData flag weather the array should take over the ownership of the data
   * @param shape std::array of type Int and length D describing the shape of the Array
   * @tparam Int integer type
   */
  template<class...A>
  explicit Array(const T* data, bool ownData, A...shape)
    : shape_({((size_t)shape)...})
    , mem((char*) data, prod(shape_)*sizeof(T), ownData)
  {
    initContiguousStrides(0);
  }

  template<class...A>
  explicit Array(T* data, bool ownData, A...shape)
    : Array((const T*) data, ownData, shape...) {}

  /*template<class Int>
  Array(std::initializer_list<Int> shape)
  {
    auto it = shape.begin();
    for(int i=0; i<D; i++,it++)
      shape_[i] = *it;

    initContiguousStrides(0);

    mem.allocate(prod(shape_)*sizeof(T));
  }*/


  template<class...A>
  explicit Array(std::initializer_list<T> data, A...shape)
    : shape_({((size_t)shape)...})
  {
    mem.allocate(prod(shape_)*sizeof(T));
    initContiguousStrides(0);

    std::vector<T> data_ = data;
    for(int i=0; i<size(); i++)
      operator[](i) = data_[i];

  }

  Array(const Array& rhs)
    : shape_(rhs.shape_)
    , strides_(rhs.strides_)
    , offset_(rhs.offset_)
    , isContiguous_(rhs.isContiguous_)
    , mem(rhs.mem)
  {
  }

  Array(const MemoryBlock& mem)
    : mem(mem)
  {
  }

  template <class U, class V>
  Array(const AbstractArray<U,D,V>& rhs)
    : Array(rhs.shape())
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];
  }

  // Assignment Operators

  template <class U, class V>
  Array& operator+= (const AbstractArray<U,D,V>& rhs)
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
  Array& operator= (const AbstractArray<U,D,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
  }

  Array& operator= (const Array& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
  }

  Array& operator*= (const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) *= rhs;

    return *this;
  }

  Array& operator=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) = rhs;

    return *this;
  }

  Array& operator+=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) += rhs;

    return *this;
  }

  size_t size() const
  {
      size_t size = shape_[0];
      for(int d=1; d<D; d++)
          size *= shape_[d];
      return size;
  }

  const std::array<size_t,D>& shape() const
  {
    return shape_;
  }

  std::array<size_t,D>& strides()
  {
    return strides_;
  }

  const std::array<size_t,D>& strides() const
  {
    return strides_;
  }

  using AbstractArray<T,D,Array<T,D> >::shape;

  size_t shape(int d) const
  {
    return shape_[d];
  }

  T& operator[](size_t index)
  {
    if(isContiguous_)
      return data()[offset_+index];
    else
    {
      return operator[](multiIndex(index));
    }
  }

  T operator[](size_t index) const
  {
    if(isContiguous_)
      return data()[offset_+index];
    else
    {
      return operator[](multiIndex(index));
    }
  }

  T& operator[](std::array<size_t,D> index)
  {
    return data()[flatIndex(index)];
  }

  T operator[](std::array<size_t,D> index) const
  {
    return data()[flatIndex(index)];
  }

  template<class...A>
  typename std::enable_if< !isSlicedArray< A... >::value,
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
        Array<T,countSlices< A... >::value>
        >::type
  {
    Array<T,countSlices< A... >::value> x(this->mem);

    //std::array<Slice, countSlices< A... >::value> slices({Slice(args)...});
    std::vector<Slice> slices = convertToSliceVector({args...});

    std::array<size_t, D> strides = strides_;

    size_t j=0;
    for(size_t i=0; i<D; i++)
    {
      // calculate offset
      x.offset_ += slices[i].start*strides_[i];
      if(slices[i].step == 0)
      {
        //strides[i+1] *= shape_[i];
      }
      else
      {
        // calculate strides
        x.strides_[j] = strides[i];
        x.strides_[j] *= slices[i].step;
        // calculate shape
        if(slices[i].end < 0)
          slices[i].end += shape()[i]+1;
        x.shape_[j] = ceil((slices[i].end - slices[i].start)
                      / ((double) slices[i].step ));
        j++;
      }
    }
    x.isContiguous_ = false;

    return x;
  }

  T* data() const
  {
    return reinterpret_cast<T*>(mem.data());
  }

  const MemoryBlock& getMem() const
  {
    return mem;
  }

private:

  template<class Int>
  size_t flatIndex(const std::array<Int,D>& index) const
  {
    size_t flatIndex = offset_;

    for(size_t i=0; i<D; i++)
      flatIndex += strides_[i]*index[i];

    return flatIndex;
  }


  std::array<size_t,D> multiIndex(const size_t& index) const
  {
    std::array<size_t,D> multIdx;
    long remaining = index;

    for(size_t i=0; i<D; i++)
    {
      multIdx[i] = remaining % shape()[i];
      remaining -= multIdx[i];
      remaining /= shape()[i];
    }
    return multIdx;
  }

  void initContiguousStrides(int mode=0)
  {
    if(mode == 1)
    {
      strides_[0] = 1;
        for(size_t i=1; i<D; i++)
          strides_[i] = strides_[i-1]*shape_[i-1];
    } else
    {
      strides_[D-1] = 1;
        for(size_t i=1; i<D; i++)
          strides_[D-1-i] = strides_[D-i]*shape_[D-i];
    }

    isContiguous_ = true;
  }

private:
  std::array<size_t,D> shape_;
  std::array<size_t,D> strides_;
  size_t offset_ = 0;
  bool isContiguous_;
  MemoryBlock mem;

  template<class U, int F> friend class Array;
  template<class U, int F, size_t DNew> friend Array<U,DNew> reshape(const Array<U,F>& x, const std::array<size_t,DNew>& shape);
};

template <class T>
using Vector = Array<T, 1 >;

template <class T>
using Matrix = Array<T, 2 >;

/*! @} */


}




#endif
