.. NumCpp documentation master file, created by sphinx-quickstart on Thu Jan  3 00:55:22 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. highlight:: c++

Introduction
==================================

NumCpp is a numerical C++ library, which allows to write numerical algorithms in a similar way as in Numpy or Matlab. It uses features of the new C++11 standard simplifying both the implementation of NumCpp and the external API. Here is a simple example of its usage::

  #include <numcpp>
  using namspace numcpp;

  int main()
  {
    Vector<double> x = ones(16);
    auto y = fft(x);
    auto z = reshape(y,4,4);

    print(z);
  }

.. leading to the output::

  Array<std::complex<double>,2> {
    shape = 4 4 
    strides = 4 1 
    data = (16,0) (0,0) (0,0) (0,0) 
           (0,0) (0,0) (0,0) (0,0) 
           (0,0) (0,0) (0,0) (0,0) 
           (0,0) (0,0) (0,0) (0,0) 
  }

Features
=========

The features of NumCpp are

* Simple API similar to Numpy/Matlab
* Truely multidimensional array object
* Clean implementation
* High performance using expression templates
* Mixing of datatypes in expressions

Content
==================

.. toctree::
   :maxdepth: 1

   concept
   performance
   tutorial
   api


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


