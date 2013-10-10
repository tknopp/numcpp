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

DynTypeArray randn(Type type, std::vector<size_t> shape)
{
  if(type == dtype< float >()) return randn<float> ( shape );
  if(type == dtype< double >()) return randn<double> ( shape );
  throw std::invalid_argument("numcpp::randn: datatype not supprted!");
}

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

DynTypeArray rand(Type type, std::vector<size_t> shape)
{
  if(type == dtype< float >()) return rand<float> ( shape );
  if(type == dtype< double >()) return rand<double> ( shape );
  throw std::invalid_argument("numcpp::rand: datatype not supprted!");
}

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
Array<T>& internal_shuffle_(Array<T>& x)
{
  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::random_shuffle ((T*)x.data(),((T*)x.data())+x.size() );
  return x;
}

DynTypeArray& shuffle_(DynTypeArray& x)
{
  CALL_TEMPLATE_FUNC_ALL_TYPES(return internal_shuffle_,x)
}

DynTypeArray shuffle(const DynTypeArray& x)
{
  auto y = copy(x);
  return shuffle_(y);
}

}
