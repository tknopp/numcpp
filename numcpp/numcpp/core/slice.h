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

  /*template<class Int>
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

  }*/

  long start;
  long end;
  long step;
};


typedef Slice slice;
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


template<typename... Args>
struct countSlices;

template<>
struct countSlices<> { static const int value = 0;};

template< class Int, typename... Args>
struct countSlices<Int, Args... >
{ static const int value = countSlices<Args...>::value;};

template<typename... Args>
struct countSlices<Slice, Args...>
{ static const int value = 1 + countSlices<Args...>::value;};

template<typename... Args>
struct isSlicedArray
{
  static const bool value = countSlices<Args...>::value != 0;
};




template<typename... Args>
struct countNonRegIndices;

template<>
struct countNonRegIndices<> { static const int value = 0;};

template< typename... Args>
struct countNonRegIndices<int, Args... >
{ static const int value = countNonRegIndices<Args...>::value;};

template< typename... Args>
struct countNonRegIndices<size_t, Args... >
{ static const int value = countNonRegIndices<Args...>::value;};

template<typename... Args>
struct countNonRegIndices<Slice, Args...>
{ static const int value = countNonRegIndices<Args...>::value;};

template< class Int, typename... Args>
struct countNonRegIndices<std::vector<Int>, Args...>
{ static const int value = 1 + countNonRegIndices<Args...>::value;};

template<  class Int, typename... Args>
struct countNonRegIndices<std::initializer_list<Int>, Args...>
{ static const int value = 1 + countNonRegIndices<Args...>::value;};
/*
template<typename... Args>
struct countNonRegIndices<std::initializer_list<int>, Args...>
{ static const int value = 1 + countNonRegIndices<Args...>::value;};*/

template<typename... Args>
struct isNonRegArray
{
  static const bool value = countNonRegIndices<Args...>::value != 0;
};




inline std::vector<size_t> convertToIndexList(const std::vector<size_t> list)
{
  return list;
}

inline std::vector<size_t> convertToIndexList(const std::vector<int> list)
{
  std::vector<size_t> x;
  for(int i=0; i<list.size(); i++)
   x.push_back(list[i]);
  return x;
}

inline std::vector<size_t> convertToIndexList(const size_t index)
{
    return std::vector<size_t> { index };
}

/*inline std::vector<size_t> convertToIndexList(const Slice& sl)
{
    std::vector<size_t> indices;

    size_t i = sl.start;

    while(i < sl.end)
    {
        indices.push_back(i);
        i += sl.step;
    }

    return indices;
}*/

inline void convertToNonRegIndicesVector(std::vector< std::vector<size_t>>& indices) {};

template<class T, typename... Args>
void convertToNonRegIndicesVector(std::vector< std::vector<size_t>>& indices, T element, Args... args)
{
    indices.push_back( convertToIndexList(element) );
    convertToNonRegIndicesVector(indices, args...);
}


}

#endif // RANGE_H
