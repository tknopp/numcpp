#ifndef NUMCPP_SOLVER_H
#define NUMCPP_SOLVER_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup linalg
@{
*/

/*!
Solve the linear system of equations A*x = b.
*/
template<class T>
Array<T> solve(const Array<T>& A, const Array<T>& b);

DynTypeArray solve(const DynTypeArray& A, const DynTypeArray& b);

/*! @} */

}




#endif
