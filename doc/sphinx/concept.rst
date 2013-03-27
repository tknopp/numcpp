***************
Concept
***************

.. highlight:: c++

.. contents::
   :depth: 1
   :local:

Simple Interface
=============

The aim of the NumCpp project is to develop a C++ library having a simple interface similar to the Numpy/Matlab API.


Multidimensional Array
=======================

Some of the excellent available C++ numerical libraries (e.g. Eigen) have the limitation that only 1D and 2D arrays (i.e. vectors and matrices)
are supported. In this regards they differ significantly from Matlab and Numpy which both provide arrays of any dimension. Example applications
where multidimensional array data is usually required is for instance in 3D magnetic resonance imaging (MRI) reconstruction. To reconstruct Cartesian
sampled 3D MRI data, one basically has to perform a 3D Fourier transform. With NumCpp, this can be done like::

  Array<cdouble,3> rawData(Nx,Ny,Nz);
  auto imageData = fft(rawData);

Unlike in Numpy, the dimension has to be known at compile time in NumCpp. This is essential to ensure high performance and simplifies
the implementation of the library in several areas. For instance, one can use the dimension to overload functions and in turn allow for 
return types depending on the dimension. For instance in NumCpp, the dot function returns a scalar when applying two vectors and returns
a vector when applying a matrix and a vector (matrix vector multiplication). If the dimension would not be a template parameter, this
would not be possible in Cpp. A further advantage is that one does not have to do runtime checks on the dimension, which is usually necessary
in Numpy and Matlab due to the dynamic nature of the underlying programming language.

Note, that NumCpp provides type aliases for 1D and 2D vectors such that a matrix vector multiplication can be written like::

  Matrix<double> A = randn(N,N);
  Vector<double> x = randn(N);

  auto y = dot(A,x)

Mixing Datatypes
=======================

In Numpy/Matlab it is completly natural to mix datatype in expressions. In many C++ libraries this is not possible.
For instance, the std::complex datatype does not support the following::

  double x = 3.0;
  std::complex<float>(2.0, 1.0);
  auto z = x + y;

By including the numcpp header ``#include <numcpp/core.h>`` the expression gets legal as the library includes the missing
mixed-type operators for the std::complex datatype. For convenience, the library has type aliases cdouble and cfloat for
std::complex<double> and std::complex<float>. In a similar way to complex scalars, it is possible to mix the datatypes of arrays in expressions::

  Matrix<cdouble> A = randn(N,N);
  Vector<double> x = linspace(0,1,N);
  Vector<int> y = ones(N);

  auto z = dot(A,x) + y*4.0f;

Clean Implementation
=======================

High Performance
=======================
