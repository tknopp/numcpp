#ifndef NUMCPP_STREAM_H
#define NUMCPP_STREAM_H

namespace numcpp
{

template<class T>
struct typeStr
{
  constexpr static const char * value = "unknown_type";
};

#define DEFINE_TYPE_STR(type) \
template<> \
struct typeStr< type > \
{ \
  constexpr static const char * value = #type ; \
};

DEFINE_TYPE_STR(bool)
DEFINE_TYPE_STR(int)
DEFINE_TYPE_STR(long)
DEFINE_TYPE_STR(float)
DEFINE_TYPE_STR(double)
DEFINE_TYPE_STR(std::complex<float>)
DEFINE_TYPE_STR(std::complex<double>)


template<class T, int D, class R>
std::ostream& operator<< (std::ostream& os,const AbstractArray<T,D,R> & x)
{
  os << "AbstractArray<"<< typeStr<T>::value
     << "," << D << "> {\n";
  os << "  shape = ";
  for(int i=0; i<D; i++)
    os << x.shape(i) << " ";
  os << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(D-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l< x.shape(D-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
  }
  os << "}\n";
  return os;
}

template<class T, int D>
std::ostream& operator<< (std::ostream& os,const Array<T,D> & x)
{
  os << "Array<"<<  typeStr<T>::value
     << "," << D << "> {\n";
  os << "  shape = ";
  for(int i=0; i<D; i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<D; i++)
    os << x.strides()[i] << " ";
  os << "\n";

  os << "  data = ";
  size_t j = 0;
  size_t nrRows = x.size() / x.shape(D-1);
  for(size_t i=0; i< nrRows; i++)
  {
    for(size_t l=0; l< x.shape(D-1); l++,j++)
      os << x[j] << " ";
    os << "\n";
    if(i<nrRows-1)
      os << "         ";
  }
  os << "}\n";
  return os;
}

template<class T>
std::ostream& operator<< (std::ostream& os,const Matrix<T> & x)
{
  os << "Matrix<"<<  typeStr<T>::value
     << "> {\n";
  os << "  shape = ";
  for(int i=0; i<2; i++)
    os << x.shape(i) << " ";
  os << "\n";
  os << "  strides = ";
  for(int i=0; i<2; i++)
    os << x.strides()[i] << " ";
  os << "\n";

  os << "  data = ";
  for(size_t i=0; i< x.shape(0); i++)
  {
    for(size_t l=0; l< x.shape(1); l++)
      os << x(i,l) << " ";
    os << "\n";
    if(i<x.shape(0)-1)
      os << "         ";
  }
  os << "}\n";
  return os;
}



}

#endif
