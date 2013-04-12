#ifndef NUMCPP_IMAGE_H
#define NUMCPP_IMAGE_H

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <unistd.h>
#include "../base.h"

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
template<class T, class R>
void export_image(const AbstractMatrix<T,R>& x, std::string filename, double winMin, double winMax, const colormap& cm = colormaps::gray)
{
  auto y = colorize(eval(x), winMin, winMax, cm);

  cairo_surface_t *surface;
  cairo_t *cr;

  bool png = false;
  if(filename.substr(filename.find_last_of(".") + 1) == "png")
    png = true;

  if(png)
    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, y.shape(0), x.shape(1));
  else
    surface = cairo_pdf_surface_create(filename.c_str(), y.shape(0), x.shape(1));

  cr = cairo_create(surface);

  cairo_surface_t *image;

  image = cairo_image_surface_create_for_data((unsigned char*) y.data(),
                                              CAIRO_FORMAT_ARGB32, y.shape(0), y.shape(1), y.shape(0) * sizeof(int32_t));

  //cairo_scale (cr, 256.0/w, 256.0/h);

  cairo_set_source_surface (cr, image, 0, 0);
  cairo_paint (cr);

  cairo_surface_destroy (image);

  if(png)
  {
    cairo_surface_flush(surface);
    cairo_surface_write_to_png(surface, filename.c_str());
  } else
  {
    cairo_show_page(cr);
  }

  cairo_surface_destroy(surface);
  cairo_destroy(cr);

}

/*!
Export the matrix \a x as pdf.

For converting of floating point values onto a colormap, one has to give the window parameters(\a winMin, \a winMax) and the colormap \a cm.
In this overload of the function export_pdf, the parameters winMin and winMax are the minimum and maximum value of \a x. The default colormap is colormaps::gray.
@overload
*/
template<class T, class R>
void export_image(const AbstractMatrix<T,R>& x, std::string filename, const colormap& cm = colormaps::gray)
{
  export_image(x, filename, min(x), max(x), cm);
}

/*template<class T, class R>
void export_pdf(const AbstractMatrix<T,R>& x, std::string filename, double winMin, double winMax, const colormap& cm = colormaps::gray)
{
  export_pdf<T>(x, filename, winMin, winMax, cm);
}

template<class T, class R>
void export_pdf(const AbstractMatrix<T,R>& x, std::string filename, const colormap& cm = colormaps::gray)
{
  export_pdf<T>(x, filename, min(x), max(x), cm);
}*/

/*! @} */

}

#endif
