******************
Base Module
******************

Overview
=======================

Trigonometric Functions
________________________


================================  ====================================================================================
:cpp:func:`sin(x) <sin>`          Trigonometric sine, element-wise
:cpp:func:`cos(x) <cos>`          Trigonometric cosine, element-wise.
================================  ====================================================================================

Reference
===========

Trigonometric Functions
________________________

.. cpp:function:: ExpressionTemplate sin(const AbstractArray\<T,D,R\>& x)

   Vecotrized sinus function. Calculates sinus of each element.
   Note that sin is implemented via an expression template and evaluated lazyly.

.. cpp:function:: ExpressionTemplate cos(const AbstractArray\<T,D,R\>& x)

   Vecotrized cosine function. Calculates cosine of each element.
   Note that cosine is implemented via an expression template and evaluated lazyly.

