#ifndef NUMCPP_COLOR_H
#define NUMCPP_COLOR_H

#include "../core.h"
#include "initializers.h"
#include "special.h"
#include <unistd.h>

namespace numcpp
{

struct color_rgba
{
 double r;
 double g;
 double b;
 double a;
};

namespace colors
{
  const color_rgba black = {0,0,0,1};
  const color_rgba white = {1,1,1,1};
  const color_rgba red = {1,0,0,1};
  const color_rgba blue = {0,0,1,1};
  const color_rgba green = {0,1,0,1};
  const color_rgba yellow = {1,1,0,1};
}

union argb_int
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

using colormap = std::vector<color_rgba>;

namespace colormaps
{
  const colormap gray = {colors::black, colors::white};
  const colormap autumn = {colors::red, colors::yellow};
  const colormap winter = {colors::blue, colors::green};
}

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

uint32_t rgbaToInt(const color_rgba& v)
{
  argb_int x;
  x.channel.r = (uint8_t) (v.r *255)  & 0x000000FF;
  x.channel.g = (uint8_t) (v.g *255)  & 0x000000FF;
  x.channel.b = (uint8_t) (v.b *255)  & 0x000000FF;
  x.channel.a = (uint8_t) (v.a *255)  & 0x000000FF;

  return x.value;
}

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
    color_rgba v = {cm[0].r+(cm[1].r-cm[0].r)*norm,
              cm[0].g+(cm[1].g-cm[0].g)*norm,
              cm[0].b+(cm[1].b-cm[0].b)*norm,
              cm[0].a+(cm[1].a-cm[0].a)*norm};
    return rgbaToInt(v);
  }

}

}


#endif // NUMCPP_COLOR_H
