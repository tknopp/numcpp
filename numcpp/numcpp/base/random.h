#ifndef NUMCPP_RANDOM_H
#define NUMCPP_RANDOM_H

#include "../core/array.h"
#include <vector>

namespace numcpp
{

/*!
@file

@addtogroup random
@brief Random array functions
@ingroup base
@{
*/

template<class T>
Array<T> randn(std::vector<size_t> shape);
DynTypeArray randn(Type type, std::vector<size_t> shape);

/*!
Create a D dimensional random array of normally distributed values.

D is the number of parameters that specifies the shape of the array.
*/
template<class T, class...A>
Array<T> randn(A...args)
{
  return randn<T>({(size_t)args...});
}

template<class...A>
DynTypeArray randn(Type type, A...args)
{
  return randn(type, {(size_t)args...});
}

template<class T>
Array<T> rand(std::vector<size_t> shape);
DynTypeArray rand(Type type, std::vector<size_t> shape);

/*!
Create a D dimensional random array of uniformly distributed values in the range [0,1).

D is the number of parameters that specifies the shape of the array.
*/
template<class T, class...A>
Array<T> rand(A...args)
{
  return rand<T>({(size_t)args...});
}

template<class...A>
DynTypeArray rand(Type type, A...args)
{
  return rand(type, {(size_t)args...});
}

template<class T>
Array<T> randi(T max, std::vector<size_t> shape);

/*!
Create a D dimensional random integer array of uniformly distributed values between 0 and max.
*/
template<class T, class...A>
Array<T> randi(T max, A...args)
{
  return randi<T>(max, {(size_t)args...});
}

/*!
Shuffle the array \a x inplace.
@see numcpp::shuffle_
*/
DynTypeArray& shuffle_(DynTypeArray& x);

/*!
Shuffle the array \a x inplace.
@see numcpp::shuffle
*/
DynTypeArray shuffle(const DynTypeArray& x);

template<class T>
Array<T>& shuffle_(Array<T>& x)
{
  return (Array<T>&) shuffle_((DynTypeArray&) x);
}

template<class T>
Array<T> shuffle(const Array<T>& x)
{
  return (Array<T>) shuffle_((DynTypeArray&) x);
}


/*! @} */

}


#endif
