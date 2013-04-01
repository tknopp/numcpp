#ifndef SPECIAL_FUNCTIONS
#define SPECIAL_FUNCTIONS

namespace numcpp
{

template <typename T>
auto sign(T x)
    -> typename std::enable_if< std::is_arithmetic< T >::value, T>::type
{
    return (T(0) < x) - (x < T(0));
}

VECTORIZE(sign, sign)

}

#endif
