#ifndef NUMCPP_SPARSEVECTOR_H
#define NUMCPP_SPARSEVECTOR_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup sparse
@{
*/

/*!
Sparse vector class
*/
template<class T>
class SparseVector : public AbstractSparseExpression<T,SparseVector<T>>
{
public:

  SparseVector(const SparseVector& rhs)
    : data_(rhs.data_)
    , index_(rhs.index_)
    , size_(rhs.size_)
  {
  }

  SparseVector(const Array<T>& data, const Array<size_t>& index, size_t N)
    : data_(data)
    , index_(index)
    , size_(N)
  {
  }

  size_t size() const
  {
    return size_;
  }

  size_t sparseSize() const
  {
    return index_.size();
  }

  size_t index(size_t n) const
  {
    return index_(n);
  }

  T& data(size_t n)
  {
    return data_(n);
  }

  T data(size_t n) const
  {
    return data_(n);
  }

  Array<T>& data()
  {
    return data_;
  }

  Array<T> data() const
  {
    return data_;
  }

private:
  size_t size_;
  Array<T> data_;
  Array<size_t> index_;
};


template<class T, class V>
typename AbstractSparseExpression<T,V>::value_type sum(const AbstractSparseExpression<T,V>& x)
{
  auto res = x.data(0);
  for(size_t n=1; n < x.sparseSize(); n++)
  {
    res += x.data(n);
  }
  return res;
}

template<class T, class U>
  COMMON_TYPE(T,U)
  dot(const SparseVector<T>& x, const SparseVector<U>& y)
{
  //return sum(x*y);
  return 0; //TODO
}

template<class T, class U, class V>
  COMMON_TYPE(T,U)
  dot(const SparseVector<T>& x, const AbstractArrayExpression<U,V>& y)
{
  T res = 0;
  for(size_t n=0; n<x.data().size(); n++)
  {
    res += x.data(n) * y(x.index(n));
  }
  return res;
}

template<class T, class U, class R>
  COMMON_TYPE(T,U)
  dot(const AbstractArrayExpression<T,R>& x, const SparseVector<U>& y)
{
  return dot(y,x);
}

template<class T>
double norm(const SparseVector<T>& x, double p=2.0)
{
  if(p == 0)
    return ( sum( abs( x.data() ) != 0 ) );
  if(std::isinf(p))
    return ( max( abs( x.data() ) ) );

  return std::pow( sum( pow( abs( x.data() ), p) ), 1./p );
}

VECTORIZE_SPARSE(conj,conj)
















#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP,OP_NAME)             \
template <class Op1, class Op2> \
class SparseArray_array_left ## OP_NAME : public  AbstractSparseExpression<decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ), SparseArray_array_left ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ) value_type; \
  SparseArray_array_left ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  size_t index(size_t i) const \
  { \
    return op2_.index(i); \
  } \
  \
  value_type data(size_t i) const \
  { \
    return op2_.data(i) OP op1_[op2_.index(i)]; \
  } \
  \
  size_t size() const\
  { \
    return op2_.size(); \
  } \
  \
  size_t sparseSize() const\
  { \
    return op2_.sparseSize(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class U, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractArrayExpression<T, Lhs>& lhs, const AbstractSparseExpression<U, Rhs>& rhs) \
    -> SparseArray_array_left ## OP_NAME < AbstractArrayExpression<T, Lhs>, AbstractSparseExpression<U, Rhs> > \
{ \
    return SparseArray_array_left ## OP_NAME < AbstractArrayExpression<T, Lhs>, AbstractSparseExpression<U, Rhs> > (lhs, rhs); \
} \
\
template <class T, class U, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractSparseExpression<T, Lhs>& lhs, const AbstractArrayExpression<U, Rhs>& rhs) \
    -> SparseArray_array_left ## OP_NAME < AbstractArrayExpression<U, Rhs>, AbstractSparseExpression<T, Lhs> > \
{ \
    return SparseArray_array_left ## OP_NAME < AbstractArrayExpression<U, Rhs>, AbstractSparseExpression<T, Lhs> > (rhs,lhs); \
} \
\
template <class Op1, class Op2> \
class SparseArray_scalar_left_ ## OP_NAME : public  AbstractSparseExpression<decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ), SparseArray_scalar_left_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ) value_type; \
  SparseArray_scalar_left_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  size_t index(size_t i) const \
  { \
    return op2_.index(i); \
  } \
  \
  value_type data(size_t i) const \
  { \
    return op2_.data(i) OP op1_; \
  } \
  \
  size_t size() const\
  { \
    return op2_.size(); \
  } \
  \
  size_t sparseSize() const\
  { \
    return op2_.sparseSize(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const Lhs& lhs, const AbstractSparseExpression<T,Rhs>& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Lhs>::type>::value, \
        SparseArray_scalar_left_ ## OP_NAME < Lhs, AbstractSparseExpression<T, Rhs> > \
        >::type  \
{ \
    return SparseArray_scalar_left_ ## OP_NAME < Lhs, AbstractSparseExpression<T, Rhs> > (lhs, rhs); \
} \
\
template <class Op1, class Op2> \
class SparseArray_scalar_right_ ## OP_NAME : public  AbstractSparseExpression<decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()), SparseArray_scalar_right_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()) value_type; \
  SparseArray_scalar_right_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  size_t index(size_t i) const \
  { \
    return op1_.index(i); \
  } \
  \
  value_type data(size_t i) const \
  { \
    return op1_.data(i) OP op2_; \
  } \
  \
  size_t size() const\
  { \
    return op1_.size(); \
  } \
  \
  size_t sparseSize() const\
  { \
    return op1_.sparseSize(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractSparseExpression<T,Lhs>& lhs, const Rhs& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Rhs>::type>::value, \
        SparseArray_scalar_right_ ## OP_NAME < AbstractSparseExpression<T, Lhs>, Rhs > \
        >::type  \
{ \
    return SparseArray_scalar_right_ ## OP_NAME < AbstractSparseExpression<T, Lhs>, Rhs > (lhs, rhs); \
}


_DEFINE_NUMCPP_BINARY_OPERATOR(+,plus)
_DEFINE_NUMCPP_BINARY_OPERATOR(-,minus)
_DEFINE_NUMCPP_BINARY_OPERATOR(*,mult)
_DEFINE_NUMCPP_BINARY_OPERATOR(/,div)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR










/*! @} */

}

#endif
