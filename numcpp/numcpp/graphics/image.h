#ifndef NUMCPP_IMAGE_H
#define NUMCPP_IMAGE_H

#include "../base/color.h"
#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup graphics
@{
*/

/*!
Export the matrix \a x as pdf.

For converting of floating point values onto a colormap, one has to give the window parameters(\a winMin, \a winMax) and the colormap \a cm.
The default colormap is colormaps::gray.
*/
template<class T>
void export_image(const Array<T>& x, std::string filename, double winMin, double winMax, colormap cm);

/*!
Export the matrix \a x as pdf.

For converting of floating point values onto a colormap, one has to give the window parameters(\a winMin, \a winMax) and the colormap \a cm.
In this overload of the function export_pdf, the parameters winMin and winMax are the minimum and maximum value of \a x. The default colormap is colormaps::gray.
@overload
*/
template<class T>
void export_image(const Array<T>& x, std::string filename, const colormap& cm = colormaps::gray)
{
  export_image(x, filename, min(x), max(x), cm);
}

/*! @} */

}

#endif
