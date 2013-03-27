#ifndef MemoryBlock_H
#define MemoryBlock_H

#include <cstdlib>
#include <ostream>
#include <map>
#include <unordered_map>
#include <iostream>

namespace numcpp
{


class MemoryBlock
{
public:

  MemoryBlock()
  {
  }

  MemoryBlock(size_t size)
  {
    if(cachingEnabled_)
    {
      /*std::cout << "MemoryBlock CTor: size = " << size << std::endl;
      std::cout << "cachesize = " << cache_.size() << std::endl;
      for (const auto& i : cache_)
        std::cout << i.first << "; " << *(i.second.second) << std::endl;*/

      auto it = cache_.find(size);
      if(it != cache_.end())
      {
        this->size = size;
        ownData = true;
        refCount = (*it).second.second;
        data_ = (*it).second.first;
        (*refCount)++;
        cache_.erase(it);
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

  MemoryBlock(char* data, size_t size, bool ownData=false)
    : data_(data)
    , size(size)
    , ownData(ownData)
  {
    if(ownData)
    {
      refCount = new size_t;
      (*refCount) = 1;
      totalAllocatedBytes += size;
    }
  }

  MemoryBlock(const MemoryBlock& rhs)
    : data_(rhs.data_)
    , size(rhs.size)
    , refCount(rhs.refCount)
    , ownData(rhs.ownData)
  {
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

  ~MemoryBlock()
  {
    if(ownData && data_)
    {
      (*refCount)--;
      if(*refCount == 0)
      {
        if(cachingEnabled_)
        {
          cache_.insert(std::pair< size_t, std::pair<char*,size_t*>>(size, std::pair<char*,size_t*>(data_,refCount)));
        } else
        {
          delete[] data_;
          delete refCount;
          totalAllocatedBytes -= size;
        }
      }
    }
  }

  void allocate(size_t size)
  {
    this->size = size;
    ownData = true;
    data_ = new char[size]; // TODO 16 Byte aligned memory
    refCount = new size_t;
    (*refCount) = 1;
    totalAllocatedBytes += size;
    numAllocations++;
  }

  char* data() const {return data_;}

  static void EnableCaching()
  {
    cachingEnabled_ = true;
  }

  static void DisableCaching()
  {
    for(auto it=cache_.begin(); it!=cache_.end(); ++it)
    {
      delete[] (*it).second.first;
      delete (*it).second.second;
      totalAllocatedBytes -= (*it).first;
    }
    cache_.clear();
    cachingEnabled_ = false;
  }

  friend std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem);

  static size_t numAllocations;

private:
  char* data_ = nullptr;
  size_t size = 0;
  size_t* refCount  = nullptr;
  bool ownData = false;

  static size_t totalAllocatedBytes;

  static bool cachingEnabled_;
  static std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;
};

size_t MemoryBlock::numAllocations = 0;

size_t MemoryBlock::totalAllocatedBytes = 0;
bool MemoryBlock::cachingEnabled_ = false;
std::unordered_multimap<size_t, std::pair<char*,size_t*>> MemoryBlock::cache_;

std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem)
{
  os << "MemoryBlock:" << std::endl;
  os << "  size = " << mem.size << std::endl;
  os << "  refCount = " << *mem.refCount << std::endl;
  os << "  dataPtr = " << reinterpret_cast<size_t>(mem.data_) << std::endl;
  os << "  totalAllocatedBytes = " << mem.totalAllocatedBytes << std::endl;

  return os;
}

void EnableCaching()
{
  MemoryBlock::EnableCaching();
}

void DisableCaching()
{
  MemoryBlock::DisableCaching();
}

}

#endif // MemoryBlock_H
