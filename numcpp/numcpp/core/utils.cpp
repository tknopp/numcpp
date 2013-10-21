#include "utils.h"

namespace numcpp
{

std::vector<size_t> multiIndex(const size_t& index, const std::vector<size_t>& shape)
{
  std::vector<size_t> multIdx(shape.size());
  std::ptrdiff_t remaining = index;

  for(std::ptrdiff_t i=shape.size()-1; i>=0; i--)
  {
    multIdx[i] = remaining % shape[i];
    remaining -= multIdx[i];
    remaining /= shape[i];
  }
  return multIdx;
}

size_t flatIndex(const std::vector<size_t>& index, const std::vector<size_t>& strides, size_t offset)
{
  for(size_t i=0; i<index.size(); i++)
    offset += strides[i]*index[i];

  return offset;
}

void copyShapeToSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis)
{
  int j=0;
  for(int i=0; i<shapeIn.size(); i++)
    if(i != axis)
    {
      shapeOut[j] = shapeIn[i];
      j++;
    }
}

void copyShapeFromSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis)
{
  int j=0;
  for(int i=0; i<shapeOut.size(); i++)
    if(i != axis)
    {
      shapeOut[i] = shapeIn[j];
      j++;
    }
}

clock::time_point tic()
{
  return clock::now();
}

size_t toc(clock::time_point t, bool print)
{
  auto diff = tic() - t;
  if(print)
  {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(diff);
    if(hours.count() > 0)
      std::cout << hours.count() << "h";
    auto min = std::chrono::duration_cast<std::chrono::hours>(diff-hours);
    if(min.count() > 0)
      std::cout << min.count() << "m";
    auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff-hours-min);
    std::cout << sec.count() / 1000.0 << "s" << std::endl;
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
}

}
