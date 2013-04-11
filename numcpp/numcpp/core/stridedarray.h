#ifndef NUMCPP_STRIDED_ARRAY_H
#define NUMCPP_STRIDED_ARRAY_H

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
class StridedArray : public AbstractStridedArray<T,D,StridedArray<T,D> >
{
public:

  // forward from base class

  using AbstractStridedArray<T,D,StridedArray<T,D> >::shape;
  using AbstractStridedArray<T,D,StridedArray<T,D> >::size;
  using AbstractStridedArray<T,D,StridedArray<T,D> >::operator[];
  using AbstractStridedArray<T,D,StridedArray<T,D> >::operator();
  using AbstractStridedArray<T,D,StridedArray<T,D> >::operator+=;
  using AbstractStridedArray<T,D,StridedArray<T,D> >::operator=;
  using AbstractStridedArray<T,D,StridedArray<T,D> >::operator*=;

  // Constructors

  StridedArray(const MemoryBlock& mem)
    : mem(mem)
  {
  }

  template <class U, class V>
  StridedArray(const AbstractArray<U,D,V>& rhs)
    : shape_(rhs.shape())
    , offset_(0)
  {
    initContiguousStrides(0);

    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];
  }

  // This one is extremly(!) important. Otherwise the assignment will make a shallow copy...
  StridedArray& operator= (const StridedArray& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
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

  T& operator[](size_t index)
  {
    return operator[](multiIndex<D>(index, shape_));
  }

  T operator[](size_t index) const
  {
    return operator[](multiIndex<D>(index, shape_));
  }

  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isSlicedArray< A... >::value ,
        StridedArray<T,countSlices< A... >::value>
        >::type
  {
    StridedArray<T,countSlices< A... >::value> x(this->mem);

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

  template<class Int>
  size_t flatIndex(const std::array<Int,D>& index) const
  {
    size_t flatIndex = offset_;

    for(size_t i=0; i<D; i++)
      flatIndex += strides_[i]*index[i];

    return flatIndex;
  }

private:

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

  template<class U, int F> friend class StridedArray;
  template<class U, int F, int O> friend class Array;
  template<class U, int F, size_t DNew> friend StridedArray<U,DNew> reshape(const StridedArray<U,F>& x, const std::array<size_t,DNew>& shape);
};


/// Alias for one-dimensional strided array (vector)
template <class T>
using StridedVector = StridedArray<T,1>;

/// Alias for two-dimensional strided array (matrix)
template <class T>
using StridedMatrix = StridedArray<T,2>;
/*! @} */


}




#endif
