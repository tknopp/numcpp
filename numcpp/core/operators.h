#ifndef COMPLEX_H
#define COMPLEX_H

namespace numcpp
{

#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP)                               \
template<class T, class U> inline                                        \
auto operator OP(std::complex<T> x, U y)                                 \
  -> typename std::enable_if<std::is_arithmetic<U>::value,               \
                  std::complex<decltype(x.real() OP y)>>::type           \
{                                                                        \
  return std::complex<decltype(x.real() OP y)>(x.real() OP y, x.imag()); \
}                                                                        \
                                                                         \
template<class T, class U> inline                                        \
auto operator OP(U y, std::complex<T> x)                                 \
  -> typename std::enable_if<std::is_arithmetic<U>::value,               \
                  std::complex<decltype(x.real() OP y)>>::type           \
{                                                                        \
  return std::complex<decltype(x.real() OP y)>(x.real() OP y, x.imag()); \
}

_DEFINE_NUMCPP_BINARY_OPERATOR(+)
_DEFINE_NUMCPP_BINARY_OPERATOR(-)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR

#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP)                               \
template<class T, class U> inline                                        \
auto operator OP(std::complex<T> x, U y)                                 \
  -> typename std::enable_if<std::is_arithmetic<U>::value,               \
                  std::complex<decltype(x.real() OP y)>>::type           \
{                                                                        \
  return std::complex<decltype(x.real() OP y)>(x.real() OP y, x.imag() OP y); \
}                                                                        \
                                                                         \
template<class T, class U> inline                                        \
auto operator OP(U y, std::complex<T> x)                                 \
  -> typename std::enable_if<std::is_arithmetic<U>::value,               \
                  std::complex<decltype(x.real() OP y)>>::type           \
{                                                                        \
  return std::complex<decltype(x.real() OP y)>(x.real() OP y, x.imag() OP y); \
}

_DEFINE_NUMCPP_BINARY_OPERATOR(*)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR


template<class T, class U> inline
auto operator /(std::complex<T> x, U y)
  -> typename std::enable_if<std::is_arithmetic<U>::value,
                  std::complex<decltype(x.real() / y)>>::type
{
  return std::complex<decltype(x.real() / y)>(x.real() / y, x.imag() / y);
}

template<class T, class U> inline
auto operator /(U y, std::complex<T> x)
  -> typename std::enable_if<std::is_arithmetic<U>::value,
                  std::complex<decltype(x.real() / y)>>::type
{
  auto denominator = pow(x.real(),2)+ pow(x.imag(),2);
  return std::complex<decltype(x.real() / y)>(x.real()*y/denominator, -x.imag()*y/denominator);
}


#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP,OP_NAME)             \
template <class Op1, class Op2> \
class Array_ ## OP_NAME : public  AbstractArray<decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ), Op1::dim, Array_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ) value_type; \
  Array_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_[i] OP op2_[i]; \
  } \
  template<class...A> \
  typename std::enable_if< !isSlicedArray< A... >::value, \
        value_type \
        >::type \
  operator()(A...args) const \
  { \
    return op1_(args...) OP op2_(args...); \
  } \
  \
  size_t size() const \
  { \
    return op1_.size(); \
  } \
  \
  const std::array<size_t,Op1::dim>& shape() const \
  { \
    return op1_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class U, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractArray<T, Lhs::dim, Lhs>& lhs, const AbstractArray<U, Rhs::dim, Rhs>& rhs) \
    -> Array_ ## OP_NAME < AbstractArray<T, Lhs::dim, Lhs>, AbstractArray<U, Rhs::dim, Rhs> > \
{ \
    return Array_ ## OP_NAME < AbstractArray<T, Lhs::dim, Lhs>, AbstractArray<U, Rhs::dim, Rhs> > (lhs, rhs); \
} \
\
template <class Op1, class Op2> \
class Array_scalar_left_ ## OP_NAME : public  AbstractArray<decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ), Op2::dim, Array_scalar_left_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ) value_type; \
  Array_scalar_left_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_ OP op2_[i]; \
  } \
  \
  size_t size() const \
  { \
    return op2_.size(); \
  } \
  \
  const std::array<size_t,Op2::dim>& shape() const \
  { \
    return op2_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const Lhs& lhs, const AbstractArray<T,Rhs::dim,Rhs>& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Lhs>::type>::value, \
        Array_scalar_left_ ## OP_NAME < Lhs, AbstractArray<T, Rhs::dim, Rhs> > \
        >::type  \
{ \
    return Array_scalar_left_ ## OP_NAME < Lhs, AbstractArray<T,Rhs::dim, Rhs> > (lhs, rhs); \
} \
\
template <class Op1, class Op2> \
class Array_scalar_right_ ## OP_NAME : public  AbstractArray<decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()), Op1::dim, Array_scalar_right_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()) value_type; \
  Array_scalar_right_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_[i] OP op2_; \
  } \
  \
  size_t size() const \
  { \
    return op1_.size(); \
  } \
  \
  const std::array<size_t,Op1::dim>& shape() const \
  { \
    return op1_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractArray<T,Lhs::dim,Lhs>& lhs, const Rhs& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Rhs>::type>::value, \
        Array_scalar_right_ ## OP_NAME < AbstractArray<T,Lhs::dim, Lhs>, Rhs > \
        >::type  \
{ \
    return Array_scalar_right_ ## OP_NAME < AbstractArray<T,Lhs::dim, Lhs>, Rhs > (lhs, rhs); \
}


_DEFINE_NUMCPP_BINARY_OPERATOR(+,plus)
_DEFINE_NUMCPP_BINARY_OPERATOR(-,minus)
_DEFINE_NUMCPP_BINARY_OPERATOR(*,mult)
_DEFINE_NUMCPP_BINARY_OPERATOR(/,div)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR



template<class T>
struct typeStr
{
  constexpr static const char * value = "unknown_type";
};

#define DEFINE_TYPE_STR(type) \
template<> \
struct typeStr< type > \
{ \
  constexpr static const char * value = #type ; \
};

DEFINE_TYPE_STR(int)
DEFINE_TYPE_STR(long)
DEFINE_TYPE_STR(float)
DEFINE_TYPE_STR(double)
DEFINE_TYPE_STR(std::complex<float>)
DEFINE_TYPE_STR(std::complex<double>)


template<class T, int D, class R>
std::ostream& operator<< (std::ostream& os,const AbstractArray<T,D,R> & x)
{
  os << "AbstractArray<"<< typeStr<T>::value
     << "," << D << "> {\n";
  os << "  shape = ";
  for(int i=0; i<D; i++)
    os << x.shape(i) << " ";
  os << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(D-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l< x.shape(D-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
  }
  os << "}\n";
  return os;
}

template<class T, int D>
std::ostream& operator<< (std::ostream& os,const Array<T,D> & x)
{
  os << "Array<"<<  typeStr<T>::value
     << "," << D << "> {\n";
  os << "  shape = ";
  for(int i=0; i<D; i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<D; i++)
    os << x.strides()[i] << " ";
  os << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(D-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l< x.shape(D-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
    if(i<nrRows-1)
      os << "         ";
  }
  os << "}\n";
  return os;
}

template<class T>
std::ostream& operator<< (std::ostream& os,const Matrix<T> & x)
{
  os << "Matrix<"<<  typeStr<T>::value
     << "> {\n";
  os << "  shape = ";
  for(int i=0; i<2; i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<2; i++)
    os << x.strides()[i] << " ";
  os << "\n";

  os << "  data = ";
  for(size_t i=0; i< x.shape(0); i++)
  {
    for(size_t l=0; l< x.shape(1); l++)
      os << x(i,l) << " ";
    os << "\n";
    if(i<x.shape(0)-1)
      os << "         ";
  }
  os << "}\n";
  return os;
}



}

#endif // COMPLEX_H
