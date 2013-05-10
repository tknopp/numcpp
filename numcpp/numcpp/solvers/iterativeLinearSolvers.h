#ifndef NUMCPP_ITERATIVE_LINEAR_SOLVERS_H
#define NUMCPP_ITERATIVE_LINEAR_SOLVERS_H

//#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup solvers
@{
*/

template<class T>
Vector<double> rowEnergy(Matrix<T> A)
{
  size_t M = shape(A,1);
  Vector<double> energy = zeros(M);

  for(size_t m=0; m<M; m++)
      energy[m] = norm(A(m,full));

  return energy;
}

/*!
Kaczmarz algorithm for solving a linear system of equations of the form \a A \a x = \b.
*/
template<class T, class U >
auto kaczmarz(Matrix<T> A, Vector<U> b, int iterations, double lambda = 0,
           bool enforceReal = false, bool enforcePositive = false)
  -> Vector< COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = COMMON_TYPE_ARRAY(A, b);
  size_t M = shape(A,0);
  size_t N = shape(A,1);

  Vector< returnType > x = zeros(N);
  Vector< returnType > residual = zeros(M);

  auto energy = rowEnergy(A);

  auto rowIndexCycle = range(0,M);

  double lambdIter = lambda;

  for(size_t l=0; l<iterations; l++)
  {
    for(size_t m=0; m<M; m++)
    {
      auto k = rowIndexCycle[m];
      if(energy[k] > 0)
      {
        auto beta = (b[k] - dot(conj(A(k,full)), x) - sqrt(lambdIter)*residual[k]) / (energy[k]*energy[k] + lambda);
        x += beta*conj(A(k,full));
        residual[k] = residual[k] + beta*sqrt(lambdIter);
      }
    }


    if(enforceReal)
      x = real(x);

    if(enforcePositive)
        x = x * sign(x);
  }

  return x;
}

// CGNR
/*!
CGNR algorithm (Conjugate Residual Normal Equation) for solving a linear system of equations of the form \a A \a x = \b.
*/
template<class Matrix, class U >
auto cgnr(const Matrix& A, const Vector<U>& b, int iterations, double lambda = 0)
  -> Vector< COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = COMMON_TYPE_ARRAY(A, b);
  size_t M = shape(A,0);
  size_t N = shape(A,1);

  Vector< returnType > x = zeros(N);
  Vector< returnType > p(N), z(N), v(M), residual(M);

  /* initialize */

  residual = b - dot(A, x);
  z = hdot(A, residual);
  p = z;

  /* loop */
  for(int j=0; j< iterations; j++)
  {
    v = dot(A, p);

    auto alpha_tmp = vdot(z,z);
    auto alpha = alpha_tmp / ( vdot(v,v) + lambda*vdot(p,p)  );

    x += alpha*p;
    residual += (-alpha)*v;
    z = hdot(A, residual) -lambda*x;

    auto beta = vdot(z,z);
    beta /= alpha_tmp;

    p = z + beta*p;
  }
  return x;
}

// Compressed Sensing
/*!
SL0 algorithm for solving a sparse linear system of equations of the form \a A \a x = \b.
*/
template<class Matrix, class U >
auto SL0(const Matrix& A, const Vector<U>& b, int iterations, int innerIterations=3, double lambda=1e-3,
      double lambdDecreaseFactor=0.5, double mu0=2)
  -> Vector< COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = COMMON_TYPE_ARRAY(A, b);
  size_t M = shape(A,0);

  Vector< returnType > x = solve(A,b);
  Vector< returnType > residual = zeros(M);

  lambda *= norm(x, INFINITY) / pow(lambdDecreaseFactor, iterations-1);

  for (int l=0; l<iterations; l++) {
    for (int u=0; u<innerIterations; u++) {
      x -= mu0 * x*exp(-pow(abs(x),2) / pow(lambda,2));
      residual = dot(A,x) - b;
      x -= solve(A,residual);
    }

    lambda *= lambdDecreaseFactor;
  }

  return x;
}

/*! @} */

}

#endif
