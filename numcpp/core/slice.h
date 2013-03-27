#ifndef SLICE_H
#define SLICE_H

#include <cstdlib>
#include <initializer_list>

namespace numcpp
{

class Slice
{
public:

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

  template<class Int>
  Slice(Int index)
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


typedef Slice slice;
const long end = -1;
const Slice all = {0,end,1};


std::vector<Slice> convertToSliceVector(std::initializer_list<Slice> slices)
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



}

#endif // RANGE_H
