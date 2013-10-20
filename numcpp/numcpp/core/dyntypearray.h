#ifndef NUMCPP_DYNTYPE_ARRAY_H
#define NUMCPP_DYNTYPE_ARRAY_H

#include "memoryblock.h"
#include "type.h"
#include "utils.h"

#include <vector>

extern template class std::vector<size_t>;

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

class DynTypeArray
{
public:
  // Constructors

  DynTypeArray(Type type, int elemSize);

  DynTypeArray(Type type, int elemSize, std::vector<size_t> shape);

  explicit DynTypeArray(Type type, int elemSize, const char* data, bool ownData, std::vector<size_t> shape);

  DynTypeArray(Type type, int elemSize, const MemoryBlock& mem, std::vector<size_t> shape);

  DynTypeArray(Type type, int elemSize, const MemoryBlock& mem, std::vector<size_t> shape,
               std::vector<size_t> strides, size_t offset);

  DynTypeArray(const DynTypeArray& rhs);

  // Member funtions

  Type elemType() const { return elemType_; }
  int elemSize() const { return elemSize_; }
  int ndims() const { return (int) shape_.size(); }
  size_t shape(int d) const { return shape_[d]; }
  const std::vector<size_t>& shape() const { return shape_; }
  size_t strides(int d) const { return strides_[d]; }
  size_t size() const { return size_; }
  size_t offset() const { return offset_; }

  char* data() const { return mem_.data(); }

  MemoryBlock& getMem() { return mem_; }
  MemoryBlock getMem() const { return mem_; }

protected:
  void initContiguousStrides(int mode=0);
  void reinitialize(const std::vector<size_t>& newShape);

  Type elemType_;
  int elemSize_;
  std::vector<size_t> shape_;
  size_t size_;
  int dim_;
  std::vector<size_t> strides_;
  size_t offset_;
  MemoryBlock mem_;
  bool isContiguous_;
};


/*! @} */


}





#endif
