#ifndef BASEARRAY_H
#define BASEARRAY_H

#include "abstractarray.h"

namespace numcpp
{

template<class T>
class BaseArray
{
public:



private:
  std::vector<size_t> shape_;
  std::strides<size_t> strides_;
};


}

#endif // BASEARRAY_H
