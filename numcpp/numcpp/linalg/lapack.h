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


#define LAPACK_GESVD_COMPLEX(typeChar,T,cast) \
inline int lapack_gesvd(char *jobu, char *jobvt, long *m, long *n,  \
          std::complex<T> *a, long *lda, T *s, std::complex<T> *u,  \
          long *ldu, std::complex<T> *vt, long *ldvt, T *work,  \
          long *lwork, T *rwork, long *info) {  \
  using namespace lapack; \
  return typeChar ## gesvd_(jobu, jobvt, m, n, ( cast *) a, lda,  ( cast *) s, ( cast *) u, ldu, \
                           ( cast *) vt, ldvt,  ( cast *) work, lwork, ( cast *) rwork, info); \
}

//LAPACK_GESVD_COMPLEX(z,float,lapack::doublecomplex, )

/*  extern "C" {
  extern int zgesvd_(char *jobu, char *jobvt, long *m, long *n,
          double *a, long *lda, double *s, double *u,
          long *ldu, double *vt, long *ldvt, double *work,
          long *lwork, double *rwork, long *info);
  }


void LinearSolverSVD::computeSVD()
{
  double* ATmp;
  long MTmp = ASVD.M = N;
  long NTmp = ASVD.N = M;
  long minNM = ASVD.minNM = MIN(NTmp,MTmp);

  ASVD.u = (double*) mpi_malloc(ASVD.u, sizeof(double)* ASVD.minNM*ASVD.M*2);
  ASVD.s = (double*) mpi_malloc(ASVD.s, sizeof(double)* ASVD.minNM);
  ASVD.v = (double*) mpi_malloc(ASVD.v, sizeof(double)* ASVD.minNM*ASVD.N*2);

  char jobu;
  char jobvt;
  if (ASVD.N < MTmp) {
    jobu = 'O';
    jobvt = 'S';
    memcpy(ASVD.u, A, sizeof(double) *ASVD.N*ASVD.M*2);
    ATmp = ASVD.u;
  } else {
    jobu = 'S';
    jobvt = 'O';
    memcpy(ASVD.v, A, sizeof(double) *ASVD.N*ASVD.M*2);
    ATmp = ASVD.v;
  }

  long lda = MAX(1,MTmp);
  long ldu = MTmp;
  long ldvt = minNM;
  long lwork = (2*minNM + MAX(NTmp,MTmp));
  double *rwork = (double*) malloc(sizeof(double)*5*ASVD.minNM);
  long info;
  if(verbose) {
    printf("minimal lwork: %ld \n", lwork);
  }
  lwork = -1;
#ifdef WIN32
  zgesvd_(&jobu, &jobvt, &MTmp, &NTmp, (doublecomplex*)ATmp, &lda, ASVD.s, (doublecomplex*)ASVD.u,
          &ldu, (doublecomplex*)ASVD.v, &ldvt, (doublecomplex*)ASVD.s, &lwork, rwork, &info);
#else
  zgesvd_(&jobu, &jobvt, &MTmp, &NTmp, ATmp, &lda, ASVD.s, ASVD.u,
          &ldu, ASVD.v, &ldvt, ASVD.s, &lwork, rwork, &info);
#endif
  lwork = (long) ASVD.s[0];
  if(verbose) {
    printf("optimal lwork: %ld; N*M: %ld \n", lwork, NTmp*MTmp*2);
  }
  double *work = (double*) malloc(sizeof(double)*lwork*2);

#ifdef WIN32
  zgesvd_(&jobu, &jobvt, &MTmp, &NTmp, (doublecomplex*)ATmp, &lda, ASVD.s, (doublecomplex*)ASVD.u,
          &ldu, (doublecomplex*)ASVD.v, &ldvt, (doublecomplex*)ASVD.s, &lwork, rwork, &info);
#else
  zgesvd_(&jobu, &jobvt, &MTmp, &NTmp, ATmp, &lda, ASVD.s, ASVD.u,
          &ldu, ASVD.v, &ldvt, work, &lwork, rwork, &info);
#endif

  ASVD.rank = ASVD.minNM;

  free(work);
  free(rwork);
}*/

/*! @} */

}




#endif
