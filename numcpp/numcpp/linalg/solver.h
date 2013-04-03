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
Vector<T> solve(const Matrix<T>& A, const Vector<T>& b)
{
  char trans = 'T';
  long M = A.shape(0);
  long N = A.shape(1);
  long one = 1;
  long lwork = -1;
  long info;
  long lda = std::max(1l,M);
  long maxNM = std::max(M,N);
  long ldb = std::max(1l,maxNM);
  T lworkcalc;

  auto B = copy(A);

  Vector<T> bwork(maxNM);
  bwork(slice(0,M)) = b;

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    &lworkcalc, &lwork, &info);

  lwork = (long) lworkcalc;
  Vector<T> work(lwork);

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    work.data(), &lwork, &info);

  Vector<T> x = bwork(slice(0,N));

  //Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic, 1>> x_(x.data(), N, 1);

  return x;
}



/*! @} */

}




#endif