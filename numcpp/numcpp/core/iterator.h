#ifndef ITERATOR_H
#define ITERATOR_H

#include <array>
#include <cstdlib>

namespace numcpp
{

template<int D>
class Iterator
{
public:
  Iterator(std::array<size_t,D> shape)
    : shape(shape)
  {
    for(int i=0; i<D; i++)
      counter[i] = 0;
  }

  void operator++(int)
  {
    //for(size_t i=0; i<D; i++)
    for(long i=D-1; i>=0; i--)
    {
      counter[i]++;
      if(counter[i] < shape[i])
        break;
      else
        counter[i] = 0;
    }
  }

  std::array<size_t,D>& operator*()
  {
    return counter;
  }

private:
    std::array<size_t,D> counter;
    std::array<size_t,D> shape;
};

}

#endif // ITERATOR_H
