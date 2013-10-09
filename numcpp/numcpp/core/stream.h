#ifndef NUMCPP_STREAM_H
#define NUMCPP_STREAM_H

#include <iosfwd>

namespace numcpp
{

template<class T>
std::ostream& operator<< (std::ostream& os, const Array<T>& x)
{
  os << "Array<"<< typeStr(x.elemType())
     << "," << x.ndims() << "> {\n";
  os << "  shape = ";
  for(int i=0; i<x.ndims(); i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<x.ndims(); i++)
    os << x.strides(i) << " ";
  os << "\n";
  os << "  offset = " << x.offset() << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(x.ndims()-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l < x.shape(x.ndims()-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
  }
  os << "}\n";
  return os;
}

}

#endif
