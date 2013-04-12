#ifndef NUMCPP_SLICE_H
#define NUMCPP_SLICE_H

#include <cstdlib>
#include <initializer_list>
#include <vector>

namespace numcpp
{

struct Slice
{
  Slice()
    : start(0)
    , end(-1)
    , step(1)
  {
  }


  Slice(long start, long end, long step=1)
    : start(start)
    , end(end)
    , step(step)
  {
  }

  //   template<class Int>
  Slice(long index)
    : start(index)
    , end(index+1)
    , step(0)
  {

  }

  Slice(unsigned long index)
    : start(index)
    , end(index+1)
    , step(0)
  {

  }

  Slice(int index)
    : start(index)
    , end(index+1)
    , step(0)
  {

  }

  Slice(unsigned int index)
    : start(index)
    , end(index+1)
    , step(0)
  {

  }

  Slice(const Slice& slice)
    : start(slice.start)
    , end(slice.end)
    , step(slice.step)
  {

  }

  template<class Int>
  Slice(std::initializer_list<Int> list)
  {
    auto it = list.begin();

    if(list.size() == 1)
    {
      start = *it;
      end = start+1;
      step = 1;
    } else if(list.size() == 2)
    {
      start = *it++;
      end = *it;
      step = 1;
    } else
    {
      start = *it++;
      end = *it++;
      step = *it;
    }

  }

  long start;
  long end;
  long step;
};


using slice = Slice;
using S = Slice;
const long end = -1;
const Slice full = {0,end,1};


inline std::vector<Slice> convertToSliceVector(std::initializer_list<Slice> slices)
{
  return std::vector<Slice>(slices);
}

/*template<class... Args>
std::array<size_t, sizeof...(Args)> toArray(Args...args)
{
  std::tuple
  return std::vector<Slice>(slices);
}*/

// count ints

template<typename... Args>
struct countInts;

template<>
struct countInts<> { static const int value = 0;};

template< class T, typename... Args>
struct countInts<T, Args... >
{ static const int value = countInts<Args...>::value;};

template<typename... Args>
struct countInts<int, Args...>
{ static const int value = 1 + countInts<Args...>::value;};

template<typename... Args>
struct countInts<unsigned int, Args...>
{ static const int value = 1 + countInts<Args...>::value;};

template<typename... Args>
struct countInts<long, Args...>
{ static const int value = 1 + countInts<Args...>::value;};

template<typename... Args>
struct countInts<unsigned long, Args...>
{ static const int value = 1 + countInts<Args...>::value;};

// count slices

template<typename... Args>
struct countSlices;

template<>
struct countSlices<> { static const int value = 0;};

template< class T, typename... Args>
struct countSlices<T, Args... >
{ static const int value = countSlices<Args...>::value;};

template<typename... Args>
struct countSlices<Slice, Args...>
{ static const int value = 1 + countSlices<Args...>::value;};

// count vectors

template<typename... Args>
struct countVectors;

template<>
struct countVectors<> { static const int value = 0;};

template<class T, typename... Args>
struct countVectors<T, Args...>
{ static const int value = countVectors<Args...>::value;};

template< class Int, typename... Args>
struct countVectors<std::vector<Int>, Args...>
{ static const int value = 1 + countVectors<Args...>::value;};


// determine type of indexing

template<typename... Args>
struct isSlicedArray
{
  static const bool value = countSlices<Args...>::value != 0 && countVectors<Args...>::value == 0;
};

template<typename... Args>
struct isNonRegArray
{ static const bool value = countVectors<Args...>::value != 0;};

template<typename... Args>
struct isElementAccess
{ static const bool value = countInts<Args...>::value == sizeof...(Args);};





using V = std::vector<int>;






inline std::vector<size_t> convertToIndexList(const std::vector<size_t> list, size_t N)
{
  return list;
}

inline std::vector<size_t> convertToIndexList(const std::vector<int> list, size_t N)
{
  std::vector<size_t> x;
  for(int i=0; i<list.size(); i++)
   x.push_back(list[i]);
  return x;
}

inline std::vector<size_t> convertToIndexList(const size_t index, size_t N)
{
    return std::vector<size_t> { index };
}

inline std::vector<size_t> convertToIndexList(Slice& sl, size_t N)
{
    std::vector<size_t> indices;

    size_t i = sl.start;

    if(sl.end < 0)
      sl.end += N+1;

    while(i < sl.end)
    {
        indices.push_back(i);
        i += sl.step;
    }

    return indices;
}


inline void convertToNonRegIndicesVector(std::vector< std::vector<size_t>>& indices, typename std::vector<size_t>::iterator it) {};

template<class T, typename... Args>
void convertToNonRegIndicesVector(std::vector< std::vector<size_t>>& indices, typename std::vector<size_t>::iterator it, T element, Args... args)
{
    indices.push_back( convertToIndexList(element, *it) );
    convertToNonRegIndicesVector(indices, ++it, args...);
}


}

#endif // RANGE_H
