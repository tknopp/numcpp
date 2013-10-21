#include "type.h"

namespace numcpp
{

template<class T>
Type dtype() { return Type::Unknown; }

template<> Type dtype<bool>() { return Type::Bool; }
template<> Type dtype<short>() { return Type::Short; }
template<> Type dtype<int>() { return Type::Int; }
template<> Type dtype<unsigned int>() { return Type::UInt; }
template<> Type dtype<long>() { return Type::Long; }
template<> Type dtype<unsigned long>() { return Type::ULong; }
template<> Type dtype<float>() { return Type::Float; }
template<> Type dtype<double>() { return Type::Double; }
template<> Type dtype<std::complex<float>>() { return Type::CFloat; }
template<> Type dtype<std::complex<double>>() { return Type::CDouble; }

std::string typeStr(Type t)
{
  switch(t)
  {
    case Type::Bool: return "bool";
    case Type::Short: return "short";
    case Type::Int: return "int";
    case Type::UInt: return "uint";
    case Type::Long: return "long";
    case Type::ULong: return "ulong";
    case Type::Float: return "float";
    case Type::Double: return "double";
    case Type::CFloat: return "cfloat";
    case Type::CDouble: return "cdouble";
    default: return "unknown";
  }
}

int nbytes(Type t)
{
  switch(t)
  {
    case Type::Bool: return sizeof(bool);
    case Type::Short: return sizeof(short);
    case Type::Int: return sizeof(int);
    case Type::UInt: return sizeof(unsigned int);
    case Type::Long: return sizeof(long);
    case Type::ULong: return sizeof(unsigned long);
    case Type::Float: return sizeof(float);
    case Type::Double: return sizeof(double);
    case Type::CFloat: return sizeof(cfloat);
    case Type::CDouble: return sizeof(cdouble);
    default: return 0;
  }
}

}
