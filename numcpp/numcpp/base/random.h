#ifndef NUMCPP_RANDOM_H
#define NUMCPP_RANDOM_H

#include "../core.h"
#include <random>
#include <algorithm>
#include <chrono>

namespace numcpp
{

/*!
@file

@addtogroup random
@brief Random array functions
@ingroup base
@{
*/

/*!
Create a D dimensional random array of normally distributed values.

D is the number of parameters that specifies the shape of the array.
*/
template<class T, class...A>
Array<T,sizeof...(A)> randn(A...args)
{
  Array<T,sizeof...(A)> x(args...);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<T> d(0,1);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

/*!
Create a D dimensional random array of uniformly distributed values in the range [0,1).

D is the number of parameters that specifies the shape of the array.
*/
template<class T, class...A>
auto rand(A...args)
    -> typename std::enable_if< std::is_floating_point< T >::value,
       Array<T,sizeof...(A)>       >::type
{
  Array<T,sizeof...(A)> x(args...);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<T> d(0.0,1.0);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

/*!
Create a D dimensional random integer array of uniformly distributed values between 0 and max.
*/
template<class T, class...A>
auto rand(T max, A...args)
    -> typename std::enable_if< std::numeric_limits<T>::is_integer,
       Array<T,sizeof...(A)>       >::type
{
  Array<T,sizeof...(A)> x(args...);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<T> d(0,max);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

/*!
Shuffle the array \a x inplace.
@see numcpp::shuffle
*/
template<class T, int D>
Array<T,D>& shuffle_(Array<T,D>& x)
{
  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  // TODO use proper iterators!!!
  //shuffle (x.data(), x.data()+x.size(), std::default_random_engine(seed));
  std::random_shuffle ((T*)x.data(),((T*)x.data())+x.size() );
  return x;
}

/*!
Shuffle the array \a x inplace.
@see numcpp::shuffle_
*/
template<class T, int D, class R>
Array<T,D> shuffle(const AbstractArray<T,D,R>& x)
{
 auto y = copy(x);
 return shuffle_(y);
}


/*! @} */

}


#endif
