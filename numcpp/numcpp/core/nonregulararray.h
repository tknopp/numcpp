#ifndef NUMCPP_NONREGULAR_ARRAY_H
#define NUMCPP_NONREGULAR_ARRAY_H

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

/// Strided array
template<class T, int D>
class NonRegularArray : public AbstractStridedArray<T,D,NonRegularArray<T,D> >
{
public:

  // forward from base class

  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::shape;
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::size;
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::operator[];
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::operator();
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::operator+=;
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::operator=;
  using AbstractStridedArray<T,D,NonRegularArray<T,D> >::operator*=;

  // Constructors

  template<class Int>
  NonRegularArray(const std::array< std::vector<size_t>, D >& indices,
        const std::array<Int,D>& strides, const size_t offset, const MemoryBlock& mem)
    : strides_(strides)
    , offset_(offset)
    , indices_(indices)
    , mem(mem)
  {
    for(int d=0; d<D; d++)
      shape_[d] = indices_[d].size();
  }

  // This one is extremly(!) important. Otherwise the assignment will make a shallow copy...
  NonRegularArray& operator= (const NonRegularArray& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

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

  const std::array<size_t,D>& strides() const
  {
    return strides_;
  }

  T& operator[](size_t index)
  {
    return operator[](multiIndex<D>(index, shape_));
  }

  T operator[](size_t index) const
  {
    return operator[](multiIndex<D>(index, shape_));
  }

  T* data() const
  {
    return reinterpret_cast<T*>(mem.data());
  }

  const MemoryBlock& getMem() const
  {
    return mem;
  }

  template<class Int>
  size_t flatIndex(const std::array<Int,D>& index) const
  {
    size_t flatIndex = offset_;

    for(size_t d=0; d<D; d++)
      flatIndex += strides_[d] * indices_[d][index[d]];

    return flatIndex;
  }

private:
  std::array<size_t,D> shape_;
  std::array<size_t,D> strides_;
  size_t offset_ = 0;
  std::array<std::vector<size_t>,D> indices_;
  MemoryBlock mem;

  template<class U, int F> friend class NonRegularArray;
  template<class U, int F, int O> friend class Array;
};

/*! @} */


}




#endif
