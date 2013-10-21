#ifndef NUMCPP_TYPE_H
#define NUMCPP_TYPE_H

#include<complex>

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

/// Alias for complex double
typedef std::complex<double> cdouble;
/// Alias for complex float
typedef std::complex<float> cfloat;

/// Enum describing the type that is used in numcpp
enum class Type
{
    Bool, Short, Int, UInt, Long, ULong, Float, Double, CFloat, CDouble, Unknown
};

/// Convert a c++ type to the numcpp Type enum
template<class T>
Type dtype();

/// String representation of the type t
std::string typeStr(Type t);

/// Number of bytes of type t
int nbytes(Type t);


/*! @} */


}

#endif
