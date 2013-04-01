#ifndef NUMCPP_RELATIONAL_H
#define NUMCPP_RELATIONAL_H

namespace numcpp
{

#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP,OP_NAME)             \
template <class Op1, class Op2> \
class Array_ ## OP_NAME : public AbstractArray<bool, Op1::dim, Array_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef bool value_type; \
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
class Array_scalar_left_ ## OP_NAME : public  AbstractArray<bool, Op2::dim, Array_scalar_left_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef bool value_type; \
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
class Array_scalar_right_ ## OP_NAME : public  AbstractArray<bool, Op1::dim, Array_scalar_right_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef bool value_type; \
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



_DEFINE_NUMCPP_BINARY_OPERATOR(<,less)
_DEFINE_NUMCPP_BINARY_OPERATOR(<=,lessEqual)
_DEFINE_NUMCPP_BINARY_OPERATOR(>,greater)
_DEFINE_NUMCPP_BINARY_OPERATOR(>=,greaterEqual)
_DEFINE_NUMCPP_BINARY_OPERATOR(==,equal)
_DEFINE_NUMCPP_BINARY_OPERATOR(!=,notEqual)
_DEFINE_NUMCPP_BINARY_OPERATOR(&&,logicalAnd)
_DEFINE_NUMCPP_BINARY_OPERATOR(||,logicalOr)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR


template<int D, class R>
bool any(const AbstractArray<bool,D,R>& x)
{
  for(size_t i=0; i<x.size(); i++)
    if(x[i])
      return true;
  return false;
}

template<int D, class R>
bool all(const AbstractArray<bool,D,R>& x)
{
  for(size_t i=0; i<x.size(); i++)
    if(!x[i])
      return false;
  return true;
}


}

#endif
