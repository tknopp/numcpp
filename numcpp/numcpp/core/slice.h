#ifndef NUMCPP_SLICE_H
#define NUMCPP_SLICE_H

#include <cstdlib>
#include <initializer_list>
#include <vector>

namespace numcpp
{


struct Slice
{
  Slice();
  Slice(ptrdiff_t start, ptrdiff_t end, ptrdiff_t step = 1);
  Slice(ptrdiff_t index);
  Slice(size_t index);
  Slice(const Slice& slice);
  template<class T>
  Slice(std::initializer_list<T> list);

  ptrdiff_t start;
  ptrdiff_t end;
  ptrdiff_t step;
};

//const long end = -1;
const Slice full = { 0, -1, 1 };

using slice = Slice;
using S = Slice;



std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices);

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
struct countArrays;

template<>
struct countArrays<> { static const int value = 0;};

template<class T, typename... Args>
struct countArrays<T, Args...>
{ static const int value = countArrays<Args...>::value;};

template< class Int, typename... Args>
struct countArrays<std::vector<Int>, Args...>
{ static const int value = 1 + countArrays<Args...>::value;};

/*template< class Int, typename... Args>
struct countArrays<Array<Int>, Args...>
{ static const int value = 1 + countArrays<Args...>::value;};*/

// determine type of indexing

template<typename... Args>
struct isSlicedArray
{
  static const bool value = countSlices<Args...>::value != 0 && countArrays<Args...>::value == 0;
};

/*template<typename... Args>
struct isNonRegArray
{ static const bool value = countArrays<Args...>::value != 0;};
*/
template<typename... Args>
struct isElementAccess
{ static const bool value = countInts<Args...>::value == sizeof...(Args);};





using V = std::vector<int>;


inline std::vector<size_t> convertToIndexList(const std::vector<size_t>& list, size_t N)
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

/*
inline void convertToNonRegIndicesArray(std::vector< std::vector<size_t>>& indices, typename std::vector<size_t>::iterator it) {};

template<class T, typename... Args>
void convertToNonRegIndicesArray(std::vector< std::vector<size_t>>& indices, typename std::vector<size_t>::iterator it, T element, Args... args)
{
    indices.push_back( convertToIndexList(element, *it) );
    convertToNonRegIndicesArray(indices, ++it, args...);
}*/


}

#endif // RANGE_H
