#ifndef NUMCPP_REARRANGE_ARRAY_H
#define NUMCPP_REARRANGE_ARRAY_H

/*!
@file

@addtogroup rearrange
@brief Rearrange array functions
@ingroup base
@{
*/


namespace numcpp
{

template<class T, int D>
Array<T,D> transpose_(Array<T,D>& x)
{
  std::reverse(x.strides().begin(), x.strides().end());
  return x;
}

template<class T, int D, class R>
Array<T,D> transpose(const AbstractArray<T,D,R>& x)
{
  auto y = copy(x);
  return transpose_(y);
}


template<class T, int D, size_t DNew>
Array<T,DNew> reshape(const Array<T,D>& x, const std::array<size_t,DNew>& shape)
{
  Array<T,DNew> y(x.getMem());
  std::copy(std::begin(shape), std::end(shape), begin(y.shape_));
  y.initContiguousStrides(0);

  return y;
}

template<class T, int D, class...A>
Array<T,sizeof...(A)> reshape(const Array<T,D>& x, A...args)
{
  std::array<size_t,sizeof...(A)> shape = {((size_t) args)...};
  return reshape(x, shape);
}

template<class T, int D, class R, class...A>
Array<T,sizeof...(A)> reshape(const AbstractArray<T,D,R>& x, A...args)
{
  Array<T,sizeof...(A)> y(args...);
  for(size_t i=0; i<y.size(); i++)
    y[i] = x[i];

  return y;
}


template<class T, class R>
Matrix<T> rotl90(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(N-n-1,m) = A(m,n);

    return B;
}

template<class T, class R>
Matrix<T> rotr90(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(n,M-m-1) = A(m,n);

    return B;
}

template<class T, class R>
Matrix<T> rot180(const AbstractMatrix<T,R>& A)
{
    auto M = A.shape(0);
    auto N = A.shape(1);

    auto B = Matrix<T>(M,N);
    for(size_t m=0; m<M; m++)
      for(size_t n=0; n<N; n++)
        B(M-m-1,N-n-1) = A(m,n);

    return B;
}

template<class T>
Vector<T> reverse_(Vector<T>& x)
{
    size_t N = x.shape(0);
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

template<class T, class R>
Vector<T> reverse(const AbstractVector<T,R>& x)
{
  return reverse_(copy(x));
}

template<class T>
Vector<T> flipdim_(Vector<T>& x, size_t)
{
  return reverse_(x);
}

template<class T, int D>
Array<T,D> flipdim_(Array<T,D>& x, size_t axis)
{
  T tmp;
  std::array<size_t,D-1> shape;

  copyShapeToSubArray<D>(x.shape(), shape, axis);

  std::array<size_t,D> index1, index2;
  Iterator<D-1> it(shape);

  for(size_t k=0; k<prod(shape); k++, it++)
  {
    copyShapeFromSubArray<D>(*it, index1, axis);
    std::copy(index1.begin(), index1.end(), index2.begin());

    size_t N = x.shape(axis);
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

template<class T, int D, class R>
Array<T,D> flipdim(const AbstractArray<T,D,R>& x, size_t axis)
{
  auto y = copy(x);
  return flipdim_(y, axis);
}

template<class T, int D>
Array<T,D> flipud_(Array<T,D>& x)
{
  return flipdim_(x, 0);
}

template<class T, int D>
Array<T,D> fliplr_(Array<T,D>& x)
{
  return flipdim_(x, 1);
}

template<class T, int D, class R>
Array<T,D> flipud(AbstractArray<T,D,R>& x)
{
  return flipdim(x, 0);
}

template<class T, int D, class R>
Array<T,D> fliplr(AbstractArray<T,D,R>& x)
{
  return flipdim(x, 1);
}

/*! @} */

}

#endif
