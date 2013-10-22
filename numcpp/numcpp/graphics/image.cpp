#include "image.h"

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cstdint>
#include "../base.h"

namespace numcpp
{


template<class T>
void export_image(const Array<T>& x, std::string filename, double winMin, double winMax, colormap cm)
{
  auto y = colorize<T>(x, winMin, winMax, cm);

  cairo_surface_t *surface;
  cairo_t *cr;

  bool png = false;
  if(filename.substr(filename.find_last_of(".") + 1) == "png")
    png = true;

  if(png)
    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, y.shape(1), y.shape(0));
  else
    surface = cairo_pdf_surface_create(filename.c_str(), y.shape(1), y.shape(0));

  cr = cairo_create(surface);

  cairo_surface_t *image;

  image = cairo_image_surface_create_for_data((unsigned char*) y.data(),
                                              CAIRO_FORMAT_ARGB32, shape(y,1), shape(y,0), shape(y,1) * sizeof(int32_t));

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

template void export_image<double>(const Array<double>& x, std::string filename, double winMin, double winMax, colormap cm);
}
