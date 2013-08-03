#ifndef NUMCPP_DIAG_H
#define NUMCPP_DIAG_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup diag
@brief Diagonal matrix functions
@ingroup base
@{
*/

// diagonal matrices

/*!
Create a diagonal matrix from a vector or a vector containing the main diagonal of a matrix
*/
template<class T>
Array<T> diag(const Array<T>& x)
{
  switch (x.ndims())
  {
    case 1:
    {
      Array<T> y = zeros(x.size(),x.size());
      for(size_t i=0; i<x.size(); i++)
          y(i,i) = x(i);
      return y;
    }
    case 2:
    {
      size_t N = shape(x,0);
      Array<T> y = zeros(N);
      for(size_t i=0; i<N; i++)
          y(i) = x(i,i);
      return y;
    }
    default:
        std::invalid_argument("diag is only supported for dim<=2!");
  }

}


/*! @} */

}




#endif
