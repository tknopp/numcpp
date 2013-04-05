#ifndef NUMCPP_COLOR_H
#define NUMCPP_COLOR_H

#include "../core.h"
#include "initializers.h"
#include "special.h"
#include <unistd.h>
#include <initializer_list>

namespace numcpp
{

/*!
@file

@addtogroup color
@brief Coloring functions for floating point arrays
@ingroup base
@{
*/

/*!
RGBA color type.
It consists of four doubles decribing the color in RGBA colorspace.
For standard colors, a string constructor exists that allows to specify
colors as strings. Here are some example color definitions:
@code
color_t c = "blue";
color_t f = {1.0, 0.5, 1.0. 1.0};
color_t h = color.green;

h.r = h.g = h.b = h.a = 1.0;
@endcode
*/
struct color_t
{
  color_t() = default;

  /// Create color from initializer list
  color_t(std::initializer_list<double> data)
  {
    r = *(data.begin());
    g = *(data.begin()+1);
    b = *(data.begin()+2);
    a = *(data.begin()+3);
  }

  /// Create color from string
  color_t(const char* str_c)
  {
    std::string str = str_c;

    if(str == "yellow")     { r=1; g=1; b=0; a=1; }
    else if(str == "green") { r=0; g=1; b=0; a=1; }
    else if(str == "blue")  { r=0; g=0; b=1; a=1; }
    else if(str == "red")   { r=1; g=0; b=0; a=1; }
    else if(str == "white") { r=1; g=1; b=1; a=1; }
    else {  r=0; g=0; b=0; a=1; }
  }

  double r;
  double g;
  double b;
  double a;
};

/// namespace containing default colors
namespace colors
{
  /// black color
  const color_t black = color_t("black");
  /// white color
  const color_t white = color_t("white");
  /// red color
  const color_t red   = color_t("red");
  /// blue color
  const color_t blue  = color_t("blue");
  /// green color
  const color_t green = color_t("green");
  /// yellow color
  const color_t yellow = color_t("yellow");
}


/*!
32 bit ARGB color type
*/
union color_argb_int
{
    uint32_t value;
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    } channel;
};

/*!
colormap is a vector of colors the describe a color gradient.
It is used for instance when mapping floating point numbers on colors.

@sa color_t @sa ::colorize
*/
using colormap = std::vector<color_t>;

/// namespace containing default colormaps
namespace colormaps
{
  /// gray colormap (black to white)
  const colormap gray = {colors::black, colors::white};
  /// autumn colormap (red to yellow)
  const colormap autumn = {colors::red, colors::yellow};
  /// winter colormap (blue to green)
  const colormap winter = {colors::blue, colors::green};
}

/*!
Colorize the floating point array \a x.
The parameters \a winMin and \a winMax specify the range that is mapped on the colormap \a cm.
*/
template<class T, int D, class R>
Array<uint32_t,D> colorize(const AbstractArray<T,D,R>& x, const T& winMin, const T& winMax, const colormap& cm)
{
  Array<uint32_t,D> coloredData(x.shape());
  for(size_t i=0; i<x.size(); i++)
  {
    coloredData[i] = colorize(x[i], winMin, winMax, cm);
  }
  return coloredData;
}

inline uint32_t rgbaToInt(const color_t& v)
{
  color_argb_int x;
  x.channel.r = (uint8_t) (v.r *255)  & 0x000000FF;
  x.channel.g = (uint8_t) (v.g *255)  & 0x000000FF;
  x.channel.b = (uint8_t) (v.b *255)  & 0x000000FF;
  x.channel.a = (uint8_t) (v.a *255)  & 0x000000FF;

  return x.value;
}

/*!
Colorize a floating point value \a x.
The parameters \a winMin and \a winMax specify the range that is mapped on the colormap \a cm.
*/
template<class T>
uint32_t colorize(const T& x, const T& winMin, const T& winMax, const colormap& cm)
{
  T norm = (x-winMin) / (winMax-winMin);

  if(norm <= 0.0)
    return rgbaToInt(cm[0]);
  else if (norm >= 1.0)
    return rgbaToInt(cm[1]);
  else
  {
    color_t v = {cm[0].r+(cm[1].r-cm[0].r)*norm,
              cm[0].g+(cm[1].g-cm[0].g)*norm,
              cm[0].b+(cm[1].b-cm[0].b)*norm,
              cm[0].a+(cm[1].a-cm[0].a)*norm};
    return rgbaToInt(v);
  }

}

/*! @} */

}


#endif
