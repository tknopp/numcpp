#ifndef NUMCPP_REARRANGE_ARRAY_H
#define NUMCPP_REARRANGE_ARRAY_H

namespace numcpp
{

/*!
@file

@addtogroup rearrange
@brief Rearrange array functions
@ingroup base
@{
*/

void reverse(std::vector<size_t>& shape);

/*!
Transpose the array \a x inplace.
The transposition will internally only swap the strides of the array and is therefore a cheap operation.
*/
template<class Array>
Array& transpose_(Array& x)
{
  reverse(x.strides());
  return x;
}


/*!
Return the transpose of the array \a x.
Note that this function will create a copy of the array \a x.
*/

template<class Array>
Array transpose(const Array& x)
{
  Array y = copy(x);
  reverse(y.strides());
  return y;
}

template<class T>
Array<T> reshape(const Array<T>& x, const std::vector<size_t>& shape)
{
  Array<T> y( x.getMem(), shape );
  return y;
}

/*!
Reshape array \a x.
*/
template<class T, class...A>
Array<T> reshape(const Array<T>& x, A...args)
{
  std::vector<size_t> shape = {((size_t) args)...};
  return reshape(x, shape);
}

template<class T>
Array<T> flatten(const Array<T>& x)
{
  Array<T> y( x.getMem(), {prod(x.shape())} );
  return y;
}

/*!
Rotate matrix by -90 degree.
*/
template<class T>
Array<T> rotl90(const Array<T>& A)
{
    auto M = shape(A,0);
    auto N = shape(A,1);

    auto B = Array<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(N-n-1,m) = A(m,n);

    return B;
}

/*!
Rotate matrix by +90 degree.
*/
template<class T>
Array<T> rotr90(const Array<T>& A)
{
    auto M = shape(A,0);
    auto N = shape(A,1);

    auto B = Array<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(n,M-m-1) = A(m,n);

    return B;
}

/*!
Rotate matrix by 180 degree.
*/
template<class T>
Array<T> rot180(const Array<T>& A)
{
    auto M = shape(A,0);
    auto N = shape(A,1);

    auto B = Array<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(M-m-1,N-n-1) = A(m,n);

    return B;
}

/*!
Reverse a vector inplace.
*/
template<class T>
Array<T>& reverse_(Array<T>& x)
{
    size_t N = shape(x,0);
    size_t j = N-1;
    T tmp;
    for(size_t n=0; n<floor(N/2.0); n++,j--)
    {
      tmp = x(j);
      x(j) = x(n);
      x(n) = tmp;
    }
    return x;
}

/*!
Reverse a vector.
*/
template<class T>
Array<T> reverse(const Array<T>& x)
{
  return reverse_(copy(x));
}

/*!
Reverse the elements of an array x along axis \a d (inplace).
*/
template<class T>
Array<T>& flipdim_(Array<T>& x, size_t axis)
{
  T tmp;
  std::vector<size_t> shape_;

  copyShapeToSubArray(x.shape(), shape_, axis);

  std::vector<size_t> index1, index2;
  Iterator it(shape_);

  for(size_t k=0; k<prod(shape_); k++, it++)
  {
    copyShapeFromSubArray(*it, index1, axis);
    std::copy(index1.begin(), index1.end(), index2.begin());

    size_t N = shape(x,axis);
    size_t j = N-1;

    for(size_t n=0; n<floor(N/2.0); n++,j--)
    {
      index1[axis] = j;
      index2[axis] = n;
      tmp = x[index1];
      x[index1] = x[index2];
      x[index2] = tmp;
    }
  }

  return x;
}

/*!
Reverse the elements of an array x along axis \a d.
*/
template<class T>
Array<T> flipdim(const Array<T>& x, size_t axis)
{
  auto y = copy(x);
  return flipdim_(y, axis);
}

/*!
Reverse the columns of a matrix (inplace).
*/
template<class T>
Array<T>& flipud_(Array<T>& x)
{
  return flipdim_(x, 0);
}

/*!
Reverse the rows of a matrix (inplace).
*/
template<class T>
Array<T>& fliplr_(Array<T>& x)
{
  return flipdim_(x, 1);
}

/*!
Reverse the columns of a matrix.
*/
template<class T>
Array<T> flipud(Array<T>& x)
{
  return flipdim(x, 0);
}

/*!
Reverse the rows of a matrix.
*/
template<class T>
Array<T> fliplr(Array<T>& x)
{
  return flipdim(x, 1);
}

/*! @} */

}

#endif
