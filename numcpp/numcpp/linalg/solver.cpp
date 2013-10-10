#include "solver.h"
#include "lapack.h"

namespace numcpp
{

template<class T>
char lapack_solve_trans();

template<> char lapack_solve_trans<float>() { return 'T'; }
template<> char lapack_solve_trans<double>() { return 'T'; }
template<> char lapack_solve_trans<std::complex<float>>() { return 'C'; }
template<> char lapack_solve_trans<std::complex<double>>() { return 'C'; }


template<class T>
Array<T> solve(const Array<T>& A, const Array<T>& b)
{
  char trans = lapack_solve_trans<T>();
  long M = shape(A,0);
  long N = shape(A,1);
  long one = 1;
  long lwork = -1;
  long info;
  long lda = std::max(1l,M);
  long maxNM = std::max(M,N);
  long ldb = std::max(1l,maxNM);
  double lworkcalc;

  auto B = copy(A);
  B = conj(B);

  Array<T> bwork(maxNM);
  bwork(slice(0,M)) = b;

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    (T*) &lworkcalc, &lwork, &info);

  lwork = (long) lworkcalc;
  Array<T> work(lwork);

  lapack_gels(&trans, &M, &N, &one, B.data(), &lda, bwork.data(), &ldb,
    work.data(), &lwork, &info);

  Array<T> x = bwork(slice(0,N));

  return x;
}

DynTypeArray solve(const DynTypeArray& A, const DynTypeArray& b)
{
  if(A.elemType() == dtype< float >()) return solve<float> ( (Array<float>&) A, (Array<float>&) b  );
  if(A.elemType() == dtype< double >()) return solve<double> ( (Array<double>&) A, (Array<double>&) b  );
  if(A.elemType() == dtype< cfloat >()) return solve<cfloat> ( (Array<cfloat>&) A, (Array<cfloat>&) b  );
  if(A.elemType() == dtype< cdouble >()) return solve<cdouble> ( (Array<cdouble>&) A, (Array<cdouble>&) b  );
  throw std::invalid_argument("numcpp::randn: datatype not supprted!");
}

}
