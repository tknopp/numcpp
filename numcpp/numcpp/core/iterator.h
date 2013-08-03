#ifndef NUMCPP_ITERATOR_H
#define NUMCPP_ITERATOR_H

namespace numcpp
{

class Iterator
{
public:
  Iterator(std::vector<size_t> shape)
    : shape(shape)
    , D(shape.size())
    , counter(D)
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

  std::vector<size_t>& operator*() { return counter; }

private:
    int D;
    std::vector<size_t> counter;
    std::vector<size_t> shape;
};

}

#endif
