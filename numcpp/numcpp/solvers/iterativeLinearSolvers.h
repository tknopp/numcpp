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

template<class Arr>
Array<double> rowEnergy(Arr A)
{
  size_t M = A.shape()[1];
  Array<double> energy = zeros(M);

  for(size_t m=0; m<M; m++)
      energy[m] = norm(A(m,full));

  return energy;
}

/*!
Kaczmarz algorithm for solving a linear system of equations of the form \a A \a x = \a b.
*/
template<class Matrix, class U >
auto kaczmarz(Matrix A, Array<U> b, int iterations, double lambda = 0,
           bool enforceReal = false, bool enforcePositive = false)
  -> Array< COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = COMMON_TYPE_ARRAY(A, b);
  size_t M = A.shape()[0];
  size_t N = A.shape()[1];

  Array< returnType > x = zeros(N);
  Array< returnType > residual = zeros(M);

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
        auto beta = (b[k] - sum(conj(A(k,full)) * x) - sqrt(lambdIter)*residual[k]) / (energy[k]*energy[k] + lambda);
        x += beta*conj(A(k,full));
        residual[k] = residual[k] + beta*sqrt(lambdIter);
      }
    }

    if(enforceReal)
      x = real(x);

  //  if(enforcePositive)
  //      x = x * sign(x);
  }

  return x;
}

// CGNR
/*!
CGNR algorithm (Conjugate Residual Normal Equation) for solving a linear system of equations of the form \a A \a x = \a b.
*/
template<class Matrix, class U >
auto cgnr(const Matrix& A, const Array<U>& b, int iterations, double lambda = 0)
  -> Array< COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = COMMON_TYPE_ARRAY(A, b);
  size_t M = A.shape()[0];
  size_t N = A.shape()[1];

  Array< returnType > x = zeros(N);
  Array< returnType > p(N), z(N), v(M), residual(M);

  /* initialize */

  residual = b - dot(A, x);
  z = hdot(A, residual);
  p = z;

  /* loop */
  for(int j=0; j< iterations; j++)
  {
    v = dot(A, p);

    auto alpha_tmp = sum(conj(z)*z);
    auto alpha = alpha_tmp / ( sum(conj(v)*v) + lambda*sum(conj(p)*p)  );

    x += alpha*p;
    residual += (-alpha)*v;
    z = hdot(A, residual) -lambda*x;

    auto beta = sum(conj(z)*z);
    beta /= alpha_tmp;

    p = z + beta*p;
  }
  return x;
}

// Compressed Sensing
/*!
SL0 algorithm for solving a sparse linear system of equations of the form \a A \a x = \a b.
*/
template<class Matrix, class U >
auto SL0(Matrix& A, const Array<U>& b, int iterations, int innerIterations=3, double lambda=2,//1e-3,
      double lambdDecreaseFactor=0.5, double mu0=2)
  -> Array< U > //COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = U;// TODO COMMON_TYPE_ARRAY(A, b);
  size_t M = A.shape()[0];

  Array< returnType > x = solve(A,b);
  Array< returnType > residual = zeros(M);

  lambda *= norm(x, INFINITY) / pow(lambdDecreaseFactor, iterations-1);

  for (int l=0; l<iterations; l++) {
    for (int u=0; u<innerIterations; u++) {
      x -= mu0 * x*exp(0-pow(abs(x),2) / pow(lambda,2));
      residual = dot(A,x) - b;
      x -= solve(A,residual);
    }

    lambda *= lambdDecreaseFactor;
  }

  return x;
}

template<class Array, class U>
Array softThreshold(const Array& x, U lambda)
{
  auto y = copy(x);

  for(size_t n=0; n<size(y); n++)
  {
    if(abs(y(n)) > 0)
      y(n) *= (abs(y(n)) - lambda) / abs(y(n));
    else
      y(n) = 0;
  }

  return y;
}

/*!
FISTA algorithm for solving a sparse linear system of equations of the form \a A \a x = \a b.
*/
template<class Matrix, class U >
auto fista(Matrix& A, const Array<U>& b, int iterations, int innerIterations=3, double lambda=1e-5,
      double lambdDecreaseFactor=1.0/1.5, double t = 1.0, double rho = 1.0)
  -> Array< U > //COMMON_TYPE_ARRAY(A, b) >
{
  using returnType = U;// TODO COMMON_TYPE_ARRAY(A, b);
  size_t M = A.shape()[0];

  Array< returnType > x = solve(A,b);
  Array< returnType > residual = zeros(M);

  auto res = copy(x);
  auto xGrad = copy(x);
  auto xOld = copy(x);

  lambda *= norm(x, INFINITY) / pow(lambdDecreaseFactor, iterations-1);

  for (int l=0; l<iterations; l++) {
    for (int u=0; u<innerIterations; u++) {
      residual = dot(A,res) - b;
      xGrad = res;
      xGrad -= rho*solve(A, residual);

      xOld = x;
      x = softThreshold(xGrad, 2*rho*lambda);

      auto beta = t - 1.;
      t = (1. + sqrt(1.+4.*pow(t,2))) / 2.;
      beta /= t;

      xOld = x - rho * xOld;
      res = x + beta * xOld;
    }
    lambda *= lambdDecreaseFactor;
  }

  return x;
}


/*! @} */

}

#endif
