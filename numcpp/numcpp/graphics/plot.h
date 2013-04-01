#ifndef NUMCPP_PLOT_H
#define NUMCPP_PLOT_H

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <unistd.h>
#include "../base.h"

namespace numcpp
{

class Figure
{
public:
  Figure(double width=600, double height=600)
    : width(width)
    , height(height)
  {
    cairo_rectangle_t extents = {0, 0, width, height};
    recording_surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, &extents);
    cr = cairo_create(recording_surface);
  }

  ~Figure()
  {
    cairo_destroy(cr);
    cairo_surface_destroy(recording_surface);
  }


  template<class T, class R>
  void plot(const AbstractVector<T,R>& y, color_rgba color = colors::black, double lw = 0.01)
  {
      plot(range(0, y.size()), y, color, lw);
  }

  template<class T, class R, class U, class Y>
  void plot(const AbstractVector<T,R>& x, const AbstractVector<U,Y>& y,
            color_rgba color = colors::black, double lw = 0.01)
  {
      cairo_save(cr);
      cairo_scale(cr, width, height);

      cairo_translate(cr, 0.05, 0.05);
      cairo_scale(cr, 0.9, 0.9);

      draw_data(cr, x, y, color, lw);

      cairo_restore(cr);
  }

  template<class T, class R>
  void imshow(const AbstractMatrix<T,R>& x, double winMin, double winMax, const colormap& cm = colormaps::gray)
  {
    cairo_save(cr);

    auto y = colorize(eval(x), winMin, winMax, cm);

    coloredImages.push_back(y);

    cairo_surface_t *image;

    image = cairo_image_surface_create_for_data((unsigned char*) y.data(),
                                              CAIRO_FORMAT_ARGB32, y.shape(0), y.shape(1), y.shape(0) * sizeof(int32_t));

    cairo_scale(cr, width, height);

    cairo_translate(cr, 0.05, 0.05);
    cairo_scale(cr, 0.9 / y.shape(0), 0.9 / y.shape(1));

    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);

    cairo_restore(cr);
  }

  template<class T, class R>
  void imshow(const AbstractMatrix<T,R>& x, const colormap& cm = colormaps::gray)
  {
    imshow(x, min(x), max(x), cm);
  }

  void save(const std::string& filename)
  {
    cairo_surface_t* surface = cairo_pdf_surface_create(filename.c_str(), width, height);
    cairo_t* cr2 = cairo_create (surface);
    cairo_set_source_surface (cr2, recording_surface, 0.0, 0.0);
    cairo_paint (cr2);

    cairo_scale(cr2, width, height);
    cairo_translate(cr2, 0.05, 0.05);
    cairo_scale(cr2, 0.9, 0.9);

    draw_frame(cr2, 0.007);

    cairo_show_page(cr2);
    cairo_destroy (cr2);
    cairo_surface_destroy(surface);
  }



private:
  cairo_surface_t *recording_surface;
  cairo_t* cr;
  double width;
  double height;

  std::vector<Matrix<uint32_t>> coloredImages;


  template<class T, class R, class U, class Y>
  void draw_data(cairo_t* cr, const AbstractVector<T,R>& x, const AbstractVector<U,Y>& y,
                 color_rgba color = colors::black, double lw = 0.01)
  {
      double maxX = max(x);
      double minX = min(x);
      double maxY = max(y);
      double minY = min(y);

      cairo_set_source_rgba (cr, color.r, color.g, color.b, color.a);
      cairo_set_line_width (cr, lw);

      cairo_move_to (cr, (x[0] - minX) / (maxX - minX), (y[0] - minY) / (maxY - minY));

      for(size_t n=1; n<x.size(); n++)
      {
          cairo_line_to (cr, (x[n] - minX) / (maxX - minX), (y[n] - minY) / (maxY - minY));
      }
      cairo_stroke (cr);
  }

  void draw_frame(cairo_t* cr, double lw)
  {
      cairo_set_line_width (cr, lw);
      cairo_set_source_rgb (cr, 0, 0, 0);
      cairo_rectangle (cr, 0, 0, 1.0, 1.0);
      cairo_stroke (cr);
  }

};



}

#endif // NUMCPP_PLOT_H
