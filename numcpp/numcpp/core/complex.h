#ifndef NUMCPP_COMPLEX_H
#define NUMCPP_COMPLEX_H

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

using std::conj;
using std::real;
using std::imag;

VECTORIZE(conj, conj)
VECTORIZE(real, real)
VECTORIZE(imag, imag)

template<class T>
T angle(const std::complex<T>& z)
{
  return std::atan2(imag(z), real(z));
}

VECTORIZE(angle, angle)

}

#endif
