#ifndef NUMCPP_SOLVER_H
#define NUMCPP_SOLVER_H

#include "../core.h"
#include "lapack.h"

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
Array<T> solve(const Array<T>& A, const Array<T>& b)
{
  char trans = 'T';
  long M = shape(A,0);
  long N = shape(A,1);
  long one = 1;
  long lwork = -1;
  long info;
  long lda = std::max(1l,M);
  long maxNM = std::max(M,N);
  long ldb = std::max(1l,maxNM);
  T lworkcalc;

  auto B = copy(A);

  Array<T> bwork(maxNM);
  bwork(slice(0,M)) = b;

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    &lworkcalc, &lwork, &info);

  lwork = (long) lworkcalc;
  Array<T> work(lwork);

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    work.data(), &lwork, &info);

  Array<T> x = bwork(slice(0,N));

  return x;
}

/*! @} */

}




#endif
