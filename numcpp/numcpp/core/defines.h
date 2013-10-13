#ifndef NUMCPP_DEFINES_H
#define NUMCPP_DEFINES_H

#include <complex>
#include <exception>
#include <cmath>

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

#define COLUMN_MAJOR_ORDER 1
#define ROW_MAJOR_ORDER 0
#define DEFAULT_MAJOR_ORDER 0

#define VECTORIZE(func,funcName) \
template <class Op> \
class Array_ ## funcName : public AbstractArrayExpression<decltype(func(std::declval< typename Op::value_type >())), Array_ ## funcName <Op>  > \
{ \
public: \
  typedef decltype(func(std::declval< typename Op::value_type >())) value_type; \
  Array_ ## funcName(const Op& a) \
  : op_(a) \
  { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return func (op_[i]); \
  } \
  \
  size_t size() const\
  { \
    return op_.size(); \
  } \
  const std::vector<size_t>& shape() const\
  { \
    return op_.shape(); \
  } \
private: \
    const Op& op_; \
}; \
\
template <class T, class Op> \
 Array_ ## funcName < AbstractArrayExpression<T, Op> > \
   funcName ( const AbstractArrayExpression<T, Op>& value) \
{ \
    return Array_ ## funcName < AbstractArrayExpression<T, Op> > (value); \
}


#define VECTORIZE_ONE_ARG(func,funcName,argType) \
template <class Op> \
class Array_ ## funcName ## _ ## argType : public  AbstractArrayExpression<decltype(func(std::declval< typename Op::value_type >(), std::declval<argType>())), Array_ ## funcName ## _ ## argType <Op>  > \
{ \
public: \
  typedef decltype(func(std::declval< typename Op::value_type >(),std::declval<argType>())) value_type; \
  Array_ ## funcName ## _ ## argType (const Op& a, argType arg) \
  : op_(a) \
  , arg_(arg) \
  { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return func (op_[i], arg_); \
  } \
  \
  size_t size() const\
  { \
    return op_.size(); \
  } \
  \
  const std::vector<size_t>& shape() const\
  { \
    return op_.shape(); \
  } \
private: \
    const Op& op_; \
    argType arg_; \
}; \
\
template <class T, class Op> \
 Array_ ## funcName ## _ ## argType < AbstractArrayExpression<T, Op> > \
   funcName ( const AbstractArrayExpression<T, Op>& value, argType arg ) \
{ \
    return Array_ ## funcName ## _ ## argType < AbstractArrayExpression<T, Op> > (value, arg); \
}

#define VECTORIZE_SPARSE(func,funcName) \
template <class Op> \
class SparseArray_ ## funcName : public AbstractSparseExpression<decltype(func(std::declval< typename Op::value_type >())), SparseArray_ ## funcName <Op>  > \
{ \
public: \
  typedef decltype(func(std::declval< typename Op::value_type >())) value_type; \
  SparseArray_ ## funcName(const Op& a) \
  : op_(a) \
  { } \
  \
  size_t index(size_t i) const \
  { \
    return op_.index(i); \
  } \
  \
  value_type data(size_t i) const \
  { \
    return func (op_.data(i)); \
  } \
  \
  size_t size() const\
  { \
    return op_.size(); \
  } \
  \
  size_t sparseSize() const\
  { \
    return op_.sparseSize(); \
  } \
private: \
    const Op& op_; \
}; \
\
template <class T, class Op> \
 SparseArray_ ## funcName < AbstractSparseExpression<T, Op> > \
   funcName ( const AbstractSparseExpression<T, Op>& value) \
{ \
    return SparseArray_ ## funcName < AbstractSparseExpression<T, Op> > (value); \
}




template<class T>
struct complexBaseType;

template<class T>
struct complexBaseType<std::complex<T>>
{
  typedef typename std::complex<T>::value_type type;
};

template<class T>
struct complexBaseType
{
  typedef T type;
};

#define COMPLEX_BASE_TYPE(T) typename complexBaseType<T>::type

template<class T>
struct doubleType;

template<class T>
struct doubleType<std::complex<T>>
{
  typedef std::complex<double> type;
};

template<class T>
struct doubleType
{
  typedef double type;
};

#define DOUBLE_TYPE(T) typename doubleType<T>::type

template<class T, class U>
struct commonArithmeticType
{
  typedef decltype( std::declval<T>() * std::declval<U>() ) type;
};

#define COMMON_TYPE(T,U) decltype( std::declval< T >() * std::declval< U >() )
#define COMMON_TYPE_ARRAY(x,y) decltype( x[0] * y[0] )

using std::ptrdiff_t;

#define CONDITIONAL_TEMPLATE_FUNC(T,func,array) if(array.elemType() == dtype< T >()) func ((Array< T >&) array );
#define CALL_TEMPLATE_FUNC_ALL_TYPES(func,array)\
  CONDITIONAL_TEMPLATE_FUNC(bool,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(int,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(unsigned int,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(long,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(unsigned long,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(float,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(cfloat,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(double,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(cdouble,func,array)

#define CALL_TEMPLATE_FUNC_CFLOAT_TYPES(func,array) \
  CONDITIONAL_TEMPLATE_FUNC(float,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(cfloat,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(double,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(cdouble,func,array)

#define CALL_TEMPLATE_FUNC_FLOAT_TYPES(func,array) \
  CONDITIONAL_TEMPLATE_FUNC(float,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(double,func,array)

#define CALL_TEMPLATE_FUNC_INT_TYPES(func,array)\
  CONDITIONAL_TEMPLATE_FUNC(bool,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(int,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(unsigned int,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(long,func,array) \
  CONDITIONAL_TEMPLATE_FUNC(unsigned long,func,array)

/*! @} */

}


#endif
