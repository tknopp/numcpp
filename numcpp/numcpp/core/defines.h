#ifndef DEFINES_H
#define DEFINES_H

#include <complex>
#include <exception>
#include <cmath>


#define VECTORIZE(func,funcName) \
template <class Op> \
class Array_ ## funcName : public  AbstractArray<decltype(func(std::declval< typename Op::value_type >())), Op::dim, Array_ ## funcName <Op>  > \
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
  size_t size() const \
  { \
    return op_.size(); \
  } \
  \
  const std::array<size_t,Op::dim>& shape() const\
  { \
    return op_.shape(); \
  } \
private: \
    const Op& op_; \
}; \
\
template <class T, class Op> \
 Array_ ## funcName < AbstractArray<T,Op::dim, Op> > \
   funcName ( const AbstractArray<T,Op::dim, Op>& value) \
{ \
    return Array_ ## funcName < AbstractArray<T,Op::dim, Op> > (value); \
}


#define VECTORIZE_ONE_ARG(func,funcName,argType) \
template <class Op> \
class Array_ ## funcName ## _ ## argType : public  AbstractArray<decltype(func(std::declval< typename Op::value_type >(), std::declval<argType>())), Op::dim, Array_ ## funcName ## _ ## argType <Op>  > \
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
  size_t size() const \
  { \
    return op_.size(); \
  } \
  \
  const std::array<size_t,Op::dim>& shape() const \
  { \
    return op_.shape(); \
  } \
private: \
    const Op& op_; \
    argType arg_; \
}; \
\
template <class T, class Op> \
 Array_ ## funcName ## _ ## argType < AbstractArray<T, Op::dim, Op> > \
   funcName ( const AbstractArray<T, Op::dim, Op>& value, argType arg ) \
{ \
    return Array_ ## funcName ## _ ## argType < AbstractArray<T, Op::dim, Op> > (value, arg); \
}




namespace numcpp
{

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

typedef std::complex<double> cdouble;
typedef std::complex<float> cfloat;

template<class T, class U>
struct commonArithmeticType
{
  typedef decltype( std::declval<T>() * std::declval<U>() ) type;
};

#define COMMON_TYPE(T,U) decltype( std::declval< T >() * std::declval< U >() )

}




#endif // DEFINES_H
