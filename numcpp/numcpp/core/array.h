#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include "memoryblock.h"
#include "utils.h"
#include "defines.h"
#include "slice.h"
#include "iterator.h"
#include "abstractarray.h"
#include "constants.h"
#include "stridedarray.h"
#include "nonregulararray.h"

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
template<class T, int D, int O=DEFAULT_MAJOR_ORDER>
class Array : public AbstractStridedArray<T,D,Array<T,D,O> >
{
public:

  // forward from base class

  using AbstractStridedArray<T,D,Array<T,D,O> >::shape;
  using AbstractStridedArray<T,D,Array<T,D,O> >::size;
  using AbstractStridedArray<T,D,Array<T,D,O> >::operator[];
  using AbstractStridedArray<T,D,Array<T,D,O> >::operator();
  using AbstractStridedArray<T,D,Array<T,D,O> >::operator+=;
  using AbstractStridedArray<T,D,Array<T,D,O> >::operator=;
  using AbstractStridedArray<T,D,Array<T,D,O> >::operator*=;

  // Constructors

  Array()
  {
    for(int d=0; d<D; d++)
      shape_[d] = 0;
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
    initContiguousStrides();
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

    initContiguousStrides();
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
    initContiguousStrides();
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
  Array(std::initializer_list<T> data, A...shape)
    : shape_({((size_t)shape)...})
    , mem(prod(shape_)*sizeof(T))
  {
    std::copy(data.begin(), data.end(), this->data());
  }

  template<class...A>
  Array(std::initializer_list<T> data)
  {
    shape_[0] = data.size();
    for(int d=1; d<D; d++)
      shape_[d] = 1;

    mem.allocate(shape_[0]*sizeof(T));
    std::copy(data.begin(), data.end(), this->data());
  }

  Array(const Array& rhs)
    : shape_(rhs.shape_)
    , strides_(rhs.strides_)
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

  // This one is extremly(!) important. Otherwise the assignment will make a shallow copy...
  Array& operator= (const Array& rhs)
  {
    if(D != ndims(rhs))
    {
      if(size() != rhs.size())
      {
        std::copy(rhs.shape().begin(), rhs.shape().end(), shape_.begin());
        mem.free();
        mem.allocate(prod(shape_)*sizeof(T));
      }
    }

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
    if(O == COLUMN_MAJOR_ORDER)
      return data()[index];
    else
    {
      return operator[](multiIndex<D>(index,shape_));
    }
  }

  T operator[](size_t index) const
  {
    if(O == COLUMN_MAJOR_ORDER)
      return data()[index];
    else
    {
      return operator[](multiIndex<D>(index,shape_));
    }
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

    return x;
  }


  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isNonRegArray< A... >::value,
        NonRegularArray<T, D - countInts< A... >::value>
        >::type
  {

    std::array< std::vector<size_t>, D - countInts< A... >::value > indices;
    std::vector< std::vector<size_t> > indexVec;

    std::vector<size_t> shapeVec;
    for(size_t i=0; i<D; i++)
      shapeVec.push_back(shape_[i]);

    convertToNonRegIndicesVector(indexVec, shapeVec.begin(), args...);

    size_t offset = 0;
    std::array<size_t, D - countInts< A... >::value > strides;

    size_t j=0;
    for(size_t i=0; i<D; i++)
    {
        if( indexVec[i].size() == 1 )
            offset += indexVec[i][0] * this->strides_[i];
        else
        {
            indices[j] = indexVec[i];
            strides[j] = this->strides_[i];
            j++;
        }
    }

    std::cout << "hallo " <<   D - countInts< A... >::value << std::endl;

    return NonRegularArray<T, D - countInts< A... >::value> (indices, strides, offset, this->mem);
  }


  T* data() const
  {
    return reinterpret_cast<T*>(mem.data());
  }

  MemoryBlock& getMem()
  {
    return mem;
  }

  MemoryBlock getMem() const
  {
    return mem;
  }

  template<class Int>
  size_t flatIndex(const std::array<Int,D>& index) const
  {
    if(O == ROW_MAJOR_ORDER)
    {
      size_t flatIndex = index[D-1];

      for(size_t i=0; i<D-1; i++)
        flatIndex += strides_[i]*index[i];

      return flatIndex;
    } else
    {
      size_t flatIndex = index[0];

      for(size_t i=1; i<D; i++)
        flatIndex += strides_[i]*index[i];

      return flatIndex;
    }
  }


protected:

  void initContiguousStrides()
  {
    if(O == COLUMN_MAJOR_ORDER)
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
  }

private:
  std::array<size_t,D> shape_;
  std::array<size_t,D> strides_;
  MemoryBlock mem;

  template<class U, int F, int Or> friend class Array;
  template<class U, int F, size_t DNew> friend Array<U,DNew> reshape(const Array<U,F>& x, const std::array<size_t,DNew>& shape);
};


/// Alias for one-dimensional array (vector)
template <class T, int O=DEFAULT_MAJOR_ORDER>
using Vector = Array<T, 1, O>;

/// Alias for two-dimensional array (matrix)
template <class T, int O=DEFAULT_MAJOR_ORDER>
using Matrix = Array<T, 2, O>;

/*! @} */


}




#endif
