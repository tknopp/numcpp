******************
Base Module
******************

.. cpp:class:: AbstractArray<T,D,Derived>

   Abstract D dimensional containing elements of type T. The template parameter Derived has the type of the concrete implementation (using the Curiously Recurring Template Pattern (CRTP)).

.. cpp:class:: Array<T,D>

   Dense D dimensional array containing elements of type T. Implements the abstract interface AbstractArray.
