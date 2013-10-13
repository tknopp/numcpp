#ifndef NUMCPP_SPARSEMATRIX_H
#define NUMCPP_SPARSEMATRIX_H

#include "../core.h"
#include "sparseVector.h"

namespace numcpp
{

/*!
@file

@addtogroup sparse
@{
*/

/*!
Sparse matrix class
*/
template<class T>
class SparseMatrixCRS : public AbstractArrayExpression<T,SparseMatrixCRS<T>>
{
public:

  SparseMatrixCRS(const SparseMatrixCRS& rhs)
    : shape_(rhs.shape_)
    , data_(rhs.data_)
    , index_(rhs.index_)
    , ptr_(rhs.ptr_)
  {
  }

  SparseMatrixCRS(const Array<T>& data, const Array<size_t>& index,
                  const Array<size_t>& ptr, size_t M, size_t N)
    : shape_({M,N})
    , data_(data)
    , index_(index)
    , ptr_(ptr)
  {

  }

  SparseMatrixCRS(const Array<T>& A, double threshold)
    : shape_(A.shape())
    , ptr_(A.shape()[0]+1)
  {
    auto absThresh = max(abs(A)) * threshold;
    size_t M = A.shape()[0];
    size_t N = A.shape()[1];

    ptr_(0) = 0;
    for(size_t m=0; m<M; m++)
    {
      ptr_(m+1) = ptr_(m);
      for(size_t n=0; n<N; n++)
      {
        if(abs(A(m,n)) >= absThresh)
          ptr_(m+1)++;
      }
    }

    data_ = zeros(ptr_(M));
    index_ = zeros(ptr_(M));

    size_t i=0;
    for(size_t m=0; m<M; m++)
    {
      for(size_t n=0; n<N; n++)
      {
        if(abs(A(m,n)) >= absThresh)
        {
          data_(i) = A(m,n);
          index_(i) = n;
          i++;
        }
      }
    }

  }

  const std::vector<size_t>& shape() const
  {
    return shape_;
  }

  size_t NCols(size_t row) const
  {
    return ptr_(row+1) - ptr_(row);
  }


  size_t index(size_t m, size_t n) const
  {
    return index_(ptr_(m) + n);
  }

  T& data(size_t m, size_t n)
  {
    return data_(ptr_(m) + n);
  }

  T data(size_t m, size_t n) const
  {
    return data_(ptr_(m) + n);
  }

  SparseVector<T> operator()(size_t m, Slice col) const
  {
    if((col.end != full.end) || (col.start != full.start) || (col.step != full.step))
      throw std::invalid_argument("SparseMatrix allows only full slicing!");
    Array<T> rowData = data_( S{ptr_(m), ptr_(m+1)} );
    Array<size_t> rowIdx = index_( S{ptr_(m), ptr_(m+1)} );
    return SparseVector<T>(rowData, rowIdx, shape_[1]);
  }

private:
  std::vector<size_t> shape_;
  Array<T> data_;
  Array<size_t> index_;
  Array<size_t> ptr_;
};


/*!
Array vector product between the sparse matrix \a A and the abstract vector \a x.
*/
template<class T, class U>
  Array< COMMON_TYPE(T,U) >
  dot(const SparseMatrixCRS<T>& A, const Array<U>& x)
{
  auto M = A.shape()[0];
  auto N = A.shape()[1];
  Array< COMMON_TYPE(T,U) > y = zeros(M);

  for(size_t m=0; m<M; m++)
  {
    T tmp = 0;
    for(size_t n=0; n<A.NCols(m); n++)
    {
      tmp += A.data(m,n) * x(A.index(m,n));
    }
    y(m) = tmp;
  }
  return y;
}

/*!
Array vector product between the adjoint of the sparse matrix \a A and the abstract vector \a x.
*/
template<class T, class U>
  Array< COMMON_TYPE(T,U) >
  hdot(const SparseMatrixCRS<T>& A, const Array<U>& x)
{
  auto M = A.shape()[0];
  auto N = A.shape()[1];
  Array< COMMON_TYPE(T,U) > y = zeros(N);

  for(size_t m=0; m<M; m++)
  {
    for(size_t n=0; n<A.NCols(m); n++)
    {
      y(A.index(m,n)) += conj( A.data(m,n) ) * x(m);
    }
  }
  return y;
}


/*! @} */

}

#endif
