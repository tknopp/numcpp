#!/usr/bin/python

from numpy import *
import timeit
import sys

def performance01(N, M):

  tic = timeit.default_timer()

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

    toc = timeit.default_timer()
    return toc-tic

def performance02(N,M):

  x = random.randn(N).astype(float32)
  A = random.randn(N,N).astype(float32)
  y = zeros(N, dtype=float32)

  tic = timeit.default_timer()

  for i in xrange(0,M):
    y += dot(A,x)*i + dot(A,x)*(1.0/(i+1))  # + dot(A,ones(N))*i;
  
  toc = timeit.default_timer()
  return toc-tic

 

l = int(sys.argv[1])
code = int(sys.argv[2])

N = 2**l
M = N

if code == 1:
  t = performance01(N,M) * 1000.
if code == 2:
  t = performance02(N,M) * 1000.

print(t)

