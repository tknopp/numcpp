#ifndef ITERATIVE_LINEAR_SOLVERS_H
#define ITERATIVE_LINEAR_SOLVERS_H

//#include "../core.h"

namespace numcpp
{

template<class T>
Vector<double> rowEnergy(Matrix<T> A)
{
  size_t M = A.shape(1);
  Vector<double> energy = zeros(M);

  for(size_t m=0; m<M; m++)
      energy[m] = norm(A(m,full));

  return energy;
}

template<class T, class U >
Vector< typename commonArithmeticType<T,U>::type >
  kaczmarz(Matrix<T> A, Vector<U> b, int iterations, double lambda,
           bool enforceReal = false, bool enforcePositive = false)
{
  typedef typename commonArithmeticType<T,U>::type returnType;
  size_t M = A.shape(0);
  size_t N = A.shape(1);

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
        auto beta = (b[k] - dot(conj(A(k,full)), x) - std::sqrt(lambdIter)*residual[k]) / (energy[k]*energy[k] + lambda);
        x += beta*conj(A(k,full));
        residual[k] = residual[k] + beta*std::sqrt(lambdIter);
      }
    }


    if(enforceReal)
      x = real(x);

    if(enforcePositive)
        x = x * sign(x);

    // && iscomplex(x)
    //  x = complex(real(x),0)
    /*end
    if enforcePositive
      x[real(x) .< 0] = 0
    end*/


  }

  return x;
}

// Compressed Sensing

template<class T, class U >
Vector< typename commonArithmeticType<T,U>::type >
  SL0(Matrix<T> A, Vector<U> b, int iterations, int innerIterations=3, double lambda=1e-3,
      double lambdDecreaseFactor=0.5, double mu0=2)
{
  typedef typename commonArithmeticType<T,U>::type returnType;
  size_t M = A.shape(0);

  Vector< returnType > x = solve(A,b);
  Vector< returnType > residual = zeros(M);

  lambda *= norm(x) / std::pow(lambdDecreaseFactor, iterations-1); // TODO normInf

  for (size_t l=0; l<iterations; l++) {
    for (size_t u=0; u<innerIterations; u++) {
      x -= mu0 * x*exp(-pow(abs(x),2) / std::pow(lambda,2));
      residual = dot(A,x) - b;
      x -= solve(A,residual);
    }

    lambda *= lambdDecreaseFactor;
  }

  return x;
}


}

#endif // ITERATIVE_LINEAR_SOLVERS_H
