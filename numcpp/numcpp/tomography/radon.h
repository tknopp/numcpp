#ifndef NUMCPP_RADON_H
#define NUMCPP_RADON_H

#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup tomography
@{
*/

/*!
Calculates the Radon transformation of the matrix \a A at angles \a gamma and detector elements
\a xi.
*/
template<class T>
Array<T> radon(const Array<T>& A, const Array<T>& xi, const Array<T>& gamma);

/*!
Filter backprojection (FBP) algorithm for reconstructing an image from its Radon transform.
\a A is the sinogram, \a gamma is the angle vector, and \a N is the number of image pixels.
*/
template<class T>
Array<T> fbp(const Array<T>& A, const Array<T>& gamma, size_t N);

/*! @} */

}


#endif
