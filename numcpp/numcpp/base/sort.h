#ifndef NUMCPP_SORT_H
#define NUMCPP_SORT_H

#include "../core/array.h"

namespace numcpp
{

template<class T>
Array<T>& sort_(Array<T>& x);

DynTypeArray& sort_(DynTypeArray& x);

template<class T>
Array<size_t> argsort(const Array<T>& x);

Array<size_t> argsort(const DynTypeArray& x);


}

#endif
