#include "dyntypearray.h"

template class std::vector<size_t>;

namespace numcpp
{

DynTypeArray::DynTypeArray(Type type, int elemSize)
    : elemType_(type)
    , elemSize_(elemSize)
    , size_(0)
    , dim_(0)
    , offset_(0)
    , isContiguous_(true)
{

}

DynTypeArray::DynTypeArray(Type type, int elemSize, std::vector<size_t> shape)
    : elemType_(type)
    , elemSize_(elemSize)
    , size_(prod(shape))
    , shape_(shape)
    , dim_(shape.size())
    , mem_(size_*elemSize_)
{
    initContiguousStrides();
}

DynTypeArray::DynTypeArray(Type type, int elemSize, const char* data, bool ownData, std::vector<size_t> shape)
    : elemType_(type)
    , elemSize_(elemSize)
    , size_(prod(shape))
    , shape_(shape)
    , dim_(shape.size())
    , mem_((char*) data, size_*elemSize, ownData)
{
    initContiguousStrides();
}

DynTypeArray::DynTypeArray(Type type, int elemSize, const MemoryBlock& mem, std::vector<size_t> shape)
    : elemType_(type)
    , elemSize_(elemSize)
    , size_(prod(shape))
    , shape_(shape)
    , dim_(shape.size())
    , mem_(mem)
{
    initContiguousStrides();
}

DynTypeArray::DynTypeArray(Type type, int elemSize, const MemoryBlock& mem, std::vector<size_t> shape,
             std::vector<size_t> strides, size_t offset)
    : elemType_(type)
    , elemSize_(elemSize)
    , size_(prod(shape))
    , shape_(shape)
    , strides_(strides)
    , dim_(shape.size())
    , mem_(mem)
    , offset_(offset)
    , isContiguous_(false)
{
}

DynTypeArray::DynTypeArray(const DynTypeArray& rhs)
    : elemType_(rhs.elemType_)
    , elemSize_(rhs.elemSize_)
    , shape_(rhs.shape_)
    , size_(rhs.size_)
    , dim_(rhs.dim_)
    , mem_(rhs.mem_)
    , offset_(rhs.offset_)
    , strides_(rhs.strides_)
    , isContiguous_(rhs.isContiguous_)
{
}

void DynTypeArray::initContiguousStrides(int mode)
{
    strides_.resize(dim_);
    if(mode == 1)
    {
        strides_[0] = 1;
        for(size_t i=1; i<dim_; i++)
            strides_[i] = strides_[i-1]*shape_[i-1];
    } else
    {
        strides_[dim_-1] = 1;
        for(size_t i=1; i<dim_; i++)
            strides_[dim_-1-i] = strides_[dim_-i]*shape_[dim_-i];
    }
    offset_ = 0;
    isContiguous_ = true;
}

void DynTypeArray::reinitialize(const std::vector<size_t>& newShape)
{
    dim_ = newShape.size();
    shape_.resize(dim_);
    std::copy(newShape.begin(), newShape.end(), shape_.begin());
    size_ = prod(newShape);
    initContiguousStrides();
    if(mem_.isAllocated())
        mem_.free();
    mem_.allocate(size_*elemSize_);
}

}
