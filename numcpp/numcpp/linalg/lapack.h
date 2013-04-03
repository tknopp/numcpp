#ifndef NUMCPP_LAPACK_H
#define NUMCPP_LAPACK_H

#include "../core.h"


namespace numcpp
{

namespace lapack
{

extern "C"
{
#include "f2c.h"
#include "clapack.h"
}

}


/*!
@file

@addtogroup linalg
@{
*/

#define LAPACK_GELS(typeChar,T,cast) \
inline int lapack_gels(char *trans, long *m, long *n, long * nrhs, T *a, long *lda, T *b, long *ldb, \
                       T *work, long *lwork, long *info) { \
  using namespace lapack; \
  return typeChar ## gels_(trans, m, n, nrhs, ( cast *) a, lda, ( cast *) b, ldb, ( cast *) work, lwork, info); \
}

LAPACK_GELS(s,float,lapack::real)
LAPACK_GELS(d,double,lapack::doublereal)
LAPACK_GELS(z,cdouble,lapack::doublecomplex)
LAPACK_GELS(c,cfloat,lapack::complex)



/*! @} */

}




#endif
