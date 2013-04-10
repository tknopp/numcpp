#!/usr/bin/python

from numpy import *
from pylab import *
import timeit
import sys
import commands

testProgs = ["./performanceNumpy.py", "../../numcpp-build/performanceNumcpp", "../../numcpp-build/performanceEigen"]

leg = ["Numpy", "NumCpp", "Eigen"]

def performance(test, L):

  t = zeros((len(testProgs),len(L)))

  fig = figure(3)

  for r in range(len(testProgs)):
    for l in range(len(L)):
      status, output = commands.getstatusoutput( testProgs[r] + " " + str(L[l]) + " " + str(test))
      print(output)
      t[r,l] = double(output)

  for r in range(len(testProgs)):    
    semilogy(2**L, t[r,:] / t[1,:])

  legend(leg,loc=2)
  fig.savefig("performance" + str(test) + ".png") 


performance(1, arange(8,13))
