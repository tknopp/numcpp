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
template<class T>
Array<T> randn(std::vector<size_t> shape)
{
  Array<T> x(shape);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<T> d(0,1);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

template Array<double> randn(std::vector<size_t> shape);
//template Array<cdouble> randn(std::vector<size_t> shape);
template Array<float> randn(std::vector<size_t> shape);
//template Array<cfloat> randn(std::vector<size_t> shape);

/*!
Create a D dimensional random array of uniformly distributed values in the range [0,1).

D is the number of parameters that specifies the shape of the array.
*/
template<class T>
Array<T> rand(std::vector<size_t> shape)
{
  Array<T> x(shape);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<T> d(0.0,1.0);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

template Array<double> rand(std::vector<size_t> shape);
//template Array<cdouble> rand(std::vector<size_t> shape);
template Array<float> rand(std::vector<size_t> shape);
//template Array<cfloat> rand(std::vector<size_t> shape);

/*!
Create a D dimensional random integer array of uniformly distributed values between 0 and max.
*/
template<class T>
Array<T> randi(T max, std::vector<size_t> shape)
{
  Array<T> x(shape);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<T> d(0,max);

  for(size_t i=0; i<x.size(); i++)
    x[i] = d(gen);

  return x;
}

template Array<int> randi(int max, std::vector<size_t> shape);
template Array<long> randi(long max, std::vector<size_t> shape);
template Array<unsigned int> randi(unsigned int max, std::vector<size_t> shape);
template Array<unsigned long> randi(unsigned long max, std::vector<size_t> shape);

template<class T>
Array<T>& shuffle_(Array<T>& x)
{
  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  // TODO use proper iterators!!!
  //shuffle (x.data(), x.data()+x.size(), std::default_random_engine(seed));
  std::random_shuffle ((T*)x.data(),((T*)x.data())+x.size() );
  return x;
}

template Array<float>& shuffle_(Array<float>& x);
template Array<double>& shuffle_(Array<double>& x);
template Array<cfloat>& shuffle_(Array<cfloat>& x);
template Array<cdouble>& shuffle_(Array<cdouble>& x);
template Array<int>& shuffle_(Array<int>& x);

template<class T>
Array<T> shuffle(const Array<T>& x)
{
 auto y = copy(x);
 return shuffle_(y);
}

template Array<float> shuffle(const Array<float>& x);
template Array<double> shuffle(const Array<double>& x);
template Array<cfloat> shuffle(const Array<cfloat>& x);
template Array<cdouble> shuffle(const Array<cdouble>& x);
template Array<int> shuffle(const Array<int>& x);

}
