#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include "dyntypearray.h"
#include "abstractexpression.h"
#include "slice.h"

#include <array>

namespace numcpp
{


template<class T>
class Array : public DynTypeArray,
                    public AbstractArrayExpression<T,Array<T> >
{
public:
  using DynTypeArray::size;
  using DynTypeArray::shape;

  Array()
    : DynTypeArray(dtype<T>(), sizeof(T)) {}

  Array(std::vector<size_t> shape)
    : DynTypeArray(dtype<T>(), sizeof(T), shape) {}

  template<class...A>
  explicit Array(A...shape)
    : DynTypeArray(dtype<T>(), sizeof(T), {((size_t)shape)...}) {}

  template<class...A>
  explicit Array(const T* data, bool ownData, A...shape)
    : DynTypeArray(dtype<T>(), sizeof(T), (const char*) data, ownData, {((size_t)shape)...}) {}

  template<class...A>
  explicit Array(T* data, bool ownData, A...shape)
    : DynTypeArray(dtype<T>(), sizeof(T), (const char*) data, ownData, {((size_t)shape)...}) {}

  explicit Array(const MemoryBlock& mem, const std::vector<size_t>& shape)
    : DynTypeArray(dtype<T>(), sizeof(T), mem, shape) {}

  explicit Array(const MemoryBlock& mem, const std::vector<size_t>& shape,
                       std::vector<size_t> strides, size_t offset)
    : DynTypeArray(dtype<T>(), sizeof(T), mem, shape, strides, offset) {}

  Array(const Array& rhs)
    : DynTypeArray(rhs) {}

  Array(const DynTypeArray& rhs)
    : DynTypeArray(rhs) {}

  template <class U, class V>
  Array(const AbstractArrayExpression<U,V>& rhs)
    : DynTypeArray(dtype<T>(), sizeof(T), rhs.shape())
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];
  }

  template<class...A>
  Array(std::vector<T> data, A...shape)
    : DynTypeArray(dtype<T>(), sizeof(T), {((size_t)shape)...})
  {
    std::copy(data.begin(), data.end(), this->data());
  }

  Array(std::initializer_list<T> data)
    : DynTypeArray(dtype<T>(), sizeof(T), {data.size()})
  {
    std::copy(data.begin(), data.end(), this->data());
  }

  // Assignment operators
  // This one is extremly(!) important. Otherwise the assignment will make a shallow copy...
  Array& operator= (const Array& rhs)
  {
    if(size() != rhs.size())
      this->reinitialize(rhs.shape());

    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
  }

  template <class U, class V>
  Array& operator= (const AbstractArrayExpression<U,V>& rhs)
  {
    if(size() != rhs.size())
      this->reinitialize(rhs.shape());

    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) = rhs[i];

    return *this;
  }

  template <class U, class V>
  Array& operator+= (const AbstractArrayExpression<U,V>& rhs)
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
  Array& operator-= (const AbstractArrayExpression<U,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) -= rhs[i];

    return *this;
  }

  template <class U, class V>
  Array& operator*= (const AbstractArrayExpression<U,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) *= rhs[i];

    return *this;
  }

  template <class U, class V>
  Array& operator/= (const AbstractArrayExpression<U,V>& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < count; ++i)
      operator[](i) /= rhs[i];

    return *this;
  }

  template <class U, class V>
  Array& operator= (const AbstractSparseExpression<U,V>& rhs)
  {
    if(size() != rhs.size())
    {
      std::cout << "Size of expressions does not fit! " << size() << " " << rhs.size() << std::endl;
      throw std::invalid_argument("Size of expressions does not fit!");
    }

    operator=(0);

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < rhs.sparseSize(); ++i)
      operator[](rhs.index(i)) = rhs.data(i);

    return *this;
  }

  template <class U, class V>
  Array& operator+= (const AbstractSparseExpression<U,V>& rhs)
  {
    if(size() != rhs.size())
    {
      std::cout << "Size of expressions does not fit! " << size() << " " << rhs.size() << std::endl;
      throw std::invalid_argument("Size of expressions does not fit!");
    }

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < rhs.sparseSize(); ++i)
      operator[](rhs.index(i)) += rhs.data(i);

    return *this;
  }

  template <class U, class V>
  Array& operator-= (const AbstractSparseExpression<U,V>& rhs)
  {
    if(size() != rhs.size())
    {
      std::cout << "Size of expressions does not fit! " << size() << " " << rhs.size() << std::endl;
      throw std::invalid_argument("Size of expressions does not fit!");
    }

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < rhs.sparseSize(); ++i)
      operator[](rhs.index(i)) -= rhs.data(i);

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

  Array& operator-=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) -= rhs;

    return *this;
  }

  Array& operator*=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) *= rhs;

    return *this;
  }

  Array& operator/=(const T& rhs)
  {
    size_t count = size();

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i< count; i++)
      operator[](i) /= rhs;

    return *this;
  }

  T* data() const
  {
    return reinterpret_cast<T*>(DynTypeArray::data());
  }

  T& operator[](std::vector<size_t> index)
  {
    return data()[flatIndex(index,strides_,offset_)];
  }

  T operator[](std::vector<size_t> index) const
  {
    return data()[flatIndex(index,strides_,offset_)];
  }

  T& operator[](size_t index)
  {
    if(isContiguous_)
      return data()[index];
    else
      return operator[](multiIndex(index,shape_));
  }

  T operator[](size_t index) const
  {
    if(isContiguous_)
      return data()[index];
    else
      return operator[](multiIndex(index,shape_));
  }
/*
  T& operator()(size_t i0)
  {
    return data()[ i0 ];
    if(__builtin_expect(!!(isContiguous_), 1))
      return data()[ i0 ];
    else
      return data()[ offset_ + strides_[0]*i0 ];
  }

  T& operator()(size_t i0, size_t i1)
  {
    if(__builtin_expect(!!(isContiguous_), 1))
      return data()[strides_[0]*i0 + i1 ];
    else
      return data()[ offset_ + strides_[0]*i0 + strides_[1]*i1 ];
  }

  T& operator()(size_t i0, size_t i1, size_t i2)
  {
    return data()[ offset_ + strides_[0]*i0 + strides_[1]*i1 + strides_[2]*i2 ];
  }

  T operator()(size_t i0) const
  {
    if(__builtin_expect(!!(isContiguous_), 1))
      return data()[ i0 ];
    else
      return data()[ offset_ + strides_[0]*i0 ];
  }

  T operator()(size_t i0, size_t i1) const
  {
    if(__builtin_expect(!!(isContiguous_), 1))
      return data()[strides_[0]*i0 + i1 ];
    else
      return data()[ offset_ + strides_[0]*i0 + strides_[1]*i1 ];
  }

  T operator()(size_t i0, size_t i1, size_t i2) const
  {
    return data()[ offset_ + strides_[0]*i0 + strides_[1]*i1 + strides_[2]*i2 ];
  }
*/
  template<class...A>
  typename std::enable_if< isElementAccess< A... >::value,
        T &
        >::type
  operator()(A...args) const
  {
    std::array< size_t , sizeof...(A) > index({((size_t)args)...});
    size_t flatIdx;

    if(__builtin_expect(!!(isContiguous_), 1))
    {
      flatIdx = index[sizeof...(A)-1];
      for(size_t i=0; i<sizeof...(A)-1; i++)
        flatIdx += strides_[i]*index[i];
    }
    else
    {
      flatIdx = offset_;
      for(size_t i=0; i<sizeof...(A); i++)
        flatIdx += strides_[i]*index[i];
    }
    return data()[flatIdx];
  }

  template<class...A>
  auto operator()(A...args) const
    -> typename std::enable_if< isSlicedArray< A... >::value ,
        Array<T>
        >::type
  {
    //std::array<Slice, countSlices< A... >::value> slices({Slice(args)...});
    std::vector<Slice> slices = convertToSliceArray({args...});

    int newDim = countSlices< A... >::value;
    std::vector<size_t> shape(newDim);
    std::vector<size_t> strides(newDim);
    size_t offset = 0;

    int j=countSlices< A... >::value-1;

    for(int i=ndims()-1; i>=0; i--)
    {
      // calculate offset
      offset += slices[i].start*strides_[i];
      if(slices[i].step == 0)
      {
        //strides[i+1] *= shape_[i];
      }
      else
      {
        // calculate strides
        strides[j] = strides_[i];
        strides[j] *= slices[i].step;
        // calculate shape
        if(slices[i].end < 0)
          slices[i].end += shape_[i]+1;
        shape[j] = ceil((slices[i].end - slices[i].start)
                      / ((double) slices[i].step ));
        j = j-1;
      }
    }

    return Array<T> (getMem(), shape, strides, offset);
  }

};

}


#endif
