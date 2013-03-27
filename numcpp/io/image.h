#ifndef NUMCPP_IMAGE_H
#define NUMCPP_IMAGE_H

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <unistd.h>
#include "../base.h"

namespace numcpp
{

template<class T>
void toimage_pdf(Matrix<T> x, std::string filename, double winMin, double winMax)
{
  auto y = colorize(x, winMin, winMax, {{1.0,0.0,0.0,0.0},{0,1.0,0,0}});

  std::cout << "colored image\n";

  print(y);

  cairo_surface_t *surface;
  cairo_t *cr;

  surface = cairo_pdf_surface_create(filename.c_str(), y.shape(0), x.shape(1));
  cr = cairo_create(surface);

  cairo_surface_t *image;

  image = cairo_image_surface_create_for_data((unsigned char*) y.data(),
                                              CAIRO_FORMAT_RGB24, y.shape(0), y.shape(1), y.shape(0) * sizeof(int32_t));

  //cairo_scale (cr, 256.0/w, 256.0/h);

  cairo_set_source_surface (cr, image, 0, 0);
  cairo_paint (cr);

  cairo_surface_destroy (image);

  cairo_show_page(cr);

  cairo_surface_destroy(surface);
  cairo_destroy(cr);

}

}

#endif // NUMCPP_IMAGE_H
