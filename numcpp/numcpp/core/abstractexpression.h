#ifndef NUMCPP_ABSTRACT_EXPRESSION_H
#define NUMCPP_ABSTRACT_EXPRESSION_H

#include <vector>

/// NumCpp Namspace
namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

template<class T>
class Array;

template<class T, class Derived>
class AbstractArrayExpression
{
public:
  typedef T value_type;

  size_t size() const
  {
    return getSelf().size();
  }

  const std::vector<size_t>& shape() const
  {
    return getSelf().shape();
  }

  T& operator[](size_t index)
  {
    return getSelf()[index];
  }

  T operator[](size_t index) const
  {
    return getSelf()[index];
  }

protected:
  Derived & getSelf() { return *static_cast<Derived *>(this); }
  Derived const & getSelf() const { return *static_cast<Derived const *>(this); }
};

template<class T, class Derived>
class AbstractSparseExpression
{
public:
  typedef T value_type;

  size_t size() const
  {
    return getSelf().size();
  }

  size_t sparseSize() const
  {
    return getSelf().sparseSize();
  }

  size_t index(size_t n) const
  {
    return getSelf().index(n);
  }

/*  T& data(size_t n)
  {
    return getSelf().data(n);
  }*/

  T data(size_t n) const
  {
    return getSelf().data(n);
  }

protected:
  Derived & getSelf() { return *static_cast<Derived *>(this); }
  Derived const & getSelf() const { return *static_cast<Derived const *>(this); }
};


/*! @} */

}

#endif
