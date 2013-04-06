from numpy import *
import timeit

def performance01():
  N=1000000
  M=10

  for l in xrange(0,10):
    x=zeros((N),dtype=cdouble)
    for i in xrange(0,M):
      x += ones((N))

    for i in xrange(0,N):
      x[i] += i

    for i in xrange(0,M):
      x += i*exp(2*pi*1j*i*linspace(0,1,N))

    for i in xrange(0,M):
      x += i


tic = timeit.default_timer()
performance01()
toc = timeit.default_timer()
print(toc-tic)
