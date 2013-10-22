#ifndef NUMCPP_COLOR_H
#define NUMCPP_COLOR_H

#include "../core.h"
#include <cstdint>
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
template<class T>
Array<uint32_t> colorize(const Array<T>& x, double winMin, double winMax, colormap cm);

uint32_t rgbaToInt(const color_t& v);

/*!
Colorize a floating point value \a x.
The parameters \a winMin and \a winMax specify the range that is mapped on the colormap \a cm.
*/
template<class T>
uint32_t colorize(const T& x, const T& winMin, const T& winMax, const colormap& cm);

/*! @} */

}


#endif
