#include "memoryblock.h"
#include <cstdlib>
#include <unordered_map>
#include <iostream>

namespace numcpp
{

class GlobalData : public AbstractGlobalData
{
public:
  virtual size_t getTotalAllocatedBytes()
  {
    return totalAllocatedBytes;
  }

  virtual bool getCachingEnabled()
  {
    return cachingEnabled_;
  }

  virtual void setCachingEnabled(bool caching)
  {
    cachingEnabled_ = caching;
  }

  size_t totalAllocatedBytes = 0;
  size_t numAllocations = 0;
  bool cachingEnabled_ = false;
  std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;

  static GlobalData& GetInstance();
};

static AbstractGlobalData& GetGlobalData()
{
   static GlobalData instance;
   return instance;
}

static GlobalData& GetGlobalDataInternal()
{
   return (GlobalData&) GetGlobalData();
}

MemoryBlock::MemoryBlock()
{
}

MemoryBlock::MemoryBlock(size_t size)
{
    if(GetGlobalData().getCachingEnabled())
    {
        /*std::cout << "MemoryBlock CTor: size = " << size << std::endl;
      std::cout << "cachesize = " << cache_.size() << std::endl;
      for (const auto& i : cache_)
        std::cout << i.first << "; " << *(i.second.second) << std::endl;*/

        auto it = GetGlobalDataInternal().cache_.find(size);
        if(it != GetGlobalDataInternal().cache_.end())
        {
            this->size = size;
            ownData = true;
            refCount = (*it).second.second;
            data_ = (*it).second.first;
            (*refCount)++;
            GetGlobalDataInternal().cache_.erase(it);
        } else
        {
            allocate(size);
        }
    } else
    {
        allocate(size);
    }

    //std::cout << "#### totalAllocatedBytes = " << totalAllocatedBytes << std::endl;
}

MemoryBlock::MemoryBlock(char* data, size_t size, bool ownData)
    : data_(data)
    , size(size)
    , ownData(ownData)
{
    if(ownData)
    {
        refCount = new size_t;
        (*refCount) = 1;
        GetGlobalDataInternal().totalAllocatedBytes += size;
    }
}

MemoryBlock::MemoryBlock(const MemoryBlock& rhs)
    : data_(rhs.data_)
    , size(rhs.size)
    , refCount(rhs.refCount)
    , ownData(rhs.ownData)
{
    //std::cout << "Create new view of data: " << refCount << " RefCount was " << (*refCount) << std::endl;
    if(ownData)
        (*refCount)++;
}

/*MemoryBlock(MemoryBlock&& rhs)
    : data_(rhs.data_)
    , size(rhs.size)
    , refCount(rhs.refCount)
    , ownData(rhs.ownData)
  {
    rhs.data_ = nullptr;
    //if(ownData)
    //(*refCount)++;
  }*/

MemoryBlock::~MemoryBlock()
{
    free();
}

void MemoryBlock::allocate(size_t size)
{
    this->size = size;
    ownData = true;
    data_ = new char[size]; // TODO 16 Byte aligned memory
    refCount = new size_t;
    (*refCount) = 1;
    GetGlobalDataInternal().totalAllocatedBytes += size;
    GetGlobalDataInternal().numAllocations++;
}

void MemoryBlock::free()
{
    if(ownData && data_)
    {
        //std::cout << "Delete view of data: " << refCount << " RefCount was " << (*refCount) << std::endl;
        (*refCount)--;
        if(*refCount == 0)
        {
            if(GetGlobalDataInternal().cachingEnabled_)
            {
                GetGlobalDataInternal().cache_.insert(std::pair< size_t, std::pair<char*,size_t*>>(size, std::pair<char*,size_t*>(data_,refCount)));
            } else
            {
                delete[] data_;
                delete refCount;
                GetGlobalDataInternal().totalAllocatedBytes -= size;
            }
        }
    }
}

void MemoryBlock::EnableCaching()
  {
    GetGlobalDataInternal().cachingEnabled_ = true;
  }

void MemoryBlock::DisableCaching()
  {
    for(auto it=GetGlobalDataInternal().cache_.begin(); it!=GetGlobalDataInternal().cache_.end(); ++it)
    {
      delete[] (*it).second.first;
      delete (*it).second.second;
      GetGlobalDataInternal().totalAllocatedBytes -= (*it).first;
    }
    GetGlobalDataInternal().cache_.clear();
    GetGlobalDataInternal().cachingEnabled_ = false;
  }

std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem)
{
  os << "MemoryBlock:" << std::endl;
  os << "  size = " << mem.size << std::endl;
  os << "  refCount = " << *mem.refCount << std::endl;
  os << "  dataPtr = " << reinterpret_cast<size_t>(mem.data_) << std::endl;
  os << "  totalAllocatedBytes = " << GetGlobalDataInternal().totalAllocatedBytes << std::endl;

  return os;
}


}
