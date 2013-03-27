******************
Core Module
******************

.. highlight:: c++

.. contents::
   :depth: 2
   :local:

AbstractArray
=============

The core module of NumCpp includes the basic datatypes of the library. The central datatype is a multidimensional array whose 
interface is described by the class AbstractArray. Basically, the **AbstractArray** is defined as follows::

  template<class T, // datatype 
           int D, // dimension
           class Derived>
  class AbstractArray
  {
  public:
    size_t size() // total number of elements
    
    size_t shape(int d) // shape of the array along axis/direction d
    
    T& operator[](size_t index) // flat index operator
    
    MagicReturnType operator()(A...args) // multi index operator
  };

For performance reasons, the member functions of the **AbstractArray** are not virtual so that do runtime polymorphism is used to implement
the base interface. Instead, we use the Curiously Recurring Template Pattern (CRTP), which allows for implementing static polymorphism
with no overhead. The basic idea of CRTP is to feed the type of the interfaces' implementation as template parameter *Derived* to the
abstract base class. In this way, the base class can call the implementation of a function in the derived class while in modern compilers
the function will be inlined.

Template parameters
____________________

Besides the Derived class, the **AbstractArray** has two template perameters. The parameter *T* is the datatype of the elements
of the array and will usally be of type ``double``, ``float``, ``complex<double>``, ``complex<float>``, ``int``, ``long``, or ``bool``.
The second template parameter is the dimension / rank of the array. In this way the dimension of the array has to be known at compile time.
While this can be an issue when building dynamic programs, it has two important advantages:

  * Performance: If D is static, the compiler can for instance unroll the loop in the multidimensional index operator.
  * Dimension Dispatch: Provide didicated implementations for dedicated dimensions

One example where it is usfull to dispatch on the dimension is the implementation of the ``dot`` function. In the one dimensional case the 
dot function should return a scalar and thus be declared as::

  template<class T, class Derived> T dot(const AbstractArray<T,1,Derived>& x, const AbstractArray<T,1,Derived>& y)

while for a matrix vector multiplication, the declaration would be::

  template<class T, class Derived> AbstractArray<T,1,Derived> dot(const AbstractArray<T,2,Derived>& x, const AbstractArray<T,1,Derived>& y)






.. cpp:class:: AbstractArray<T,D,Derived>

   Abstract D dimensional containing elements of type T. The template parameter Derived has the type of the concrete implementation (using the Curiously Recurring Template Pattern (CRTP)).

.. cpp:class:: Array<T,D>

   Dense D dimensional array containing elements of type T. Implements the abstract interface AbstractArray.
