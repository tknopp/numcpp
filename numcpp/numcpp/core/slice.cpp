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

Slice::Slice(std::initializer_list<long> list)
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

std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices)
{
  return std::vector<Slice>(slices);
}

}
