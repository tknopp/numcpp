#include "color.h"

namespace numcpp
{

uint32_t rgbaToInt(const color_t& v)
{
  color_argb_int x;
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
    color_t v = {cm[0].r+(cm[1].r-cm[0].r)*norm,
                 cm[0].g+(cm[1].g-cm[0].g)*norm,
                 cm[0].b+(cm[1].b-cm[0].b)*norm,
                 cm[0].a+(cm[1].a-cm[0].a)*norm};
    return rgbaToInt(v);
  }

}

template uint32_t colorize(const float& x, const float& winMin, const float& winMax, const colormap& cm);
template uint32_t colorize(const double& x, const double& winMin, const double& winMax, const colormap& cm);

template<class T>
Array<uint32_t> colorize(const Array<T>& x, double winMin, double winMax, colormap cm)
{
  Array<uint32_t> coloredData(x.shape());
  for(size_t i=0; i<x.size(); i++)
  {
    coloredData[i] = colorize<T>(x[i], winMin, winMax, cm);
  }
  return coloredData;
}

template Array<uint32_t> colorize(const Array<float>& x, double winMin, double winMax, colormap cm);
template Array<uint32_t> colorize(const Array<double>& x, double winMin, double winMax, colormap cm);


}
