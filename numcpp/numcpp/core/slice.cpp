#include "slice.h"

namespace numcpp
{

Slice::Slice()
    : start(0)
    , end(-1)
    , step(1)
{
}


Slice::Slice(long start, long end, long step)
    : start(start)
    , end(end)
    , step(step)
{
}

Slice::Slice(long index)
    : start(index)
    , end(index+1)
    , step(0)
{

}

Slice::Slice(unsigned long index)
    : start(index)
    , end(index+1)
    , step(0)
{

}

Slice::Slice(int index)
    : start(index)
    , end(index+1)
    , step(0)
{

}

Slice::Slice(unsigned int index)
    : start(index)
    , end(index+1)
    , step(0)
{

}

Slice::Slice(const Slice& slice)
    : start(slice.start)
    , end(slice.end)
    , step(slice.step)
{

}

template<class T>
Slice::Slice(std::initializer_list<T> list)
{
    auto it = list.begin();

    if(list.size() == 1)
    {
        start = (long)*it;
        end = start+1;
        step = 1;
    } else if(list.size() == 2)
    {
        start = (long)*it++;
        end = (long)*it;
        step = 1;
    } else
    {
        start = (long)*it++;
        end = (long)*it++;
        step = (long)*it;
    }
}
template Slice::Slice(std::initializer_list<int> list);
template Slice::Slice(std::initializer_list<long> list);
template Slice::Slice(std::initializer_list<unsigned int> list);
template Slice::Slice(std::initializer_list<unsigned long> list);

std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices)
{
  return std::vector<Slice>(slices);
}

}
