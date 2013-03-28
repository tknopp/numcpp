.. NumCpp documentation master file, created by sphinx-quickstart on Thu Jan  3 00:55:22 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. highlight:: c++

Introduction
==================================
NumCpp is a numerical C++ library, which allows to write numerical algorithms in a similar way as in Numpy or Matlab. 
It uses features of the new C++11 standard simplifying both the implementation of NumCpp and the external API. 
Here is a simple example of its usage::
  #include <numcpp>
  using namspace numcpp;

  int main()
  {
    Vector<double> x = ones(16);
    auto y = fft(x);
    auto z = reshape(y,4,4);

    print(z);
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
   commandTable
   api

License
==================
The core functionallity of NumCpp is licensed under the `LGPL <http://www.gnu.org/copyleft/lesser.html>`_.
However, some functionality requires to link against software that is licensed under the GPL 
`GPL <http://www.gnu.org/licenses/gpl.html>`_. For instance the fft function internally uses the FFTW library,
which is licensed under the GPL. If you plan to use NumCpp in a closed source software, please `contact us <mailto:tobias.knopp@gmail.com>`_
to obtain a version of NumCpp where all GPL modules are replaced by GPL-free modules.

Contact
==================
For questions and discussions you can use the `google group numcpp
<https://groups.google.com/forum/?hl=de&fromgroups#!forum/numcpp>`_.

If you are missing functionality in NumCpp you can either contribute to the project,
or request features on a consulting basis (please `contact us <mailto:tobias.knopp@gmail.com>`_)

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


