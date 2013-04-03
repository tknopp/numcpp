from numpy import *

N=70
M=10

x=zeros((N*N*N),dtype=cdouble)
for i in xrange(0,M):
  #x = i*exp(2*pi*1j*i*linspace(0,1,N*N*N)).reshape((N,N,N))
  x += ones((N*N*N))

for i in xrange(0,N*N*N):
  x[i] += i

for i in xrange(0,M):
  x += i*exp(2*pi*1j*i*linspace(0,1,N*N*N))#.reshape((N,N,N))

x.tofile("/tmp/testNumpy2.dat")
