#include "coreFunc.h"
#include <exception>
#include <sstream>
#include <stdexcept>

namespace numcpp
{

int ndims(const DynTypeArray& x)
{
  return x.ndims();
}

size_t shape(const DynTypeArray& x, int d)
{
  return x.shape(d);
}

size_t size(const DynTypeArray& x)
{
  return x.size();
}

DynTypeArray similar(const DynTypeArray& x)
{
  return DynTypeArray(x.elemType(), x.elemSize(), x.shape());
}

void checkDim(const DynTypeArray& x, int dim)
{
  if(x.ndims() != dim)
  {
    std::stringstream s;
    s << "numcpp::Array has an invalid dimension of " << x.ndims() << " while it should  have dim = " << dim << "!";
    throw std::invalid_argument(s.str());
  }
}

}
