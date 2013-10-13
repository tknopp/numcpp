#include "sort.h"
#include "../core.h"
#include <algorithm>

namespace numcpp
{

template<class T>
Array<T>& sort_(Array<T>& x)
{
  std::sort(x.data(), x.data()+x.size());
}

DynTypeArray& sort_(DynTypeArray& x)
{
  CALL_TEMPLATE_FUNC_INT_TYPES(return sort_,x)
  CALL_TEMPLATE_FUNC_FLOAT_TYPES(return sort_,x)
}

template<class T>
Array<size_t> argsort(const Array<T>& x)
{
  Array<size_t> idx = range(0, x.size());

  std::sort(idx.data(), idx.data()+idx.size(),
    [&x](size_t i1, size_t i2) {return x[i1] < x[i2];});

  return idx;
}

Array<size_t> argsort(const DynTypeArray& x)
{
  CALL_TEMPLATE_FUNC_INT_TYPES(return argsort,x)
  CALL_TEMPLATE_FUNC_FLOAT_TYPES(return argsort,x)
}


}
