#ifndef MemoryBlock_H
#define MemoryBlock_H

#include <cstdlib>
#include <ostream>
#include <map>
#include <unordered_map>
#include <iostream>

namespace numcpp
{

class GlobalData
{
public:
  size_t totalAllocatedBytes = 0;
  size_t numAllocations = 0;
  bool cachingEnabled_ = false;
  std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;

  static GlobalData& GetInstance();
};

inline GlobalData &GlobalData::GetInstance()
{
   static GlobalData instance;
   return instance;
}

class MemoryBlock
{
public:

  MemoryBlock()
  {
  }

  MemoryBlock(size_t size)
  {
    if(GlobalData::GetInstance().cachingEnabled_)
    {
      /*std::cout << "MemoryBlock CTor: size = " << size << std::endl;
      std::cout << "cachesize = " << cache_.size() << std::endl;
      for (const auto& i : cache_)
        std::cout << i.first << "; " << *(i.second.second) << std::endl;*/

      auto it = GlobalData::GetInstance().cache_.find(size);
      if(it != GlobalData::GetInstance().cache_.end())
      {
        this->size = size;
        ownData = true;
        refCount = (*it).second.second;
        data_ = (*it).second.first;
        (*refCount)++;
        GlobalData::GetInstance().cache_.erase(it);
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
      GlobalData::GetInstance().totalAllocatedBytes += size;
    }
  }

  MemoryBlock(const MemoryBlock& rhs)
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

  ~MemoryBlock()
  {
    if(ownData && data_)
    {
      //std::cout << "Delete view of data: " << refCount << " RefCount was " << (*refCount) << std::endl;
      (*refCount)--;
      if(*refCount == 0)
      {
        if(GlobalData::GetInstance().cachingEnabled_)
        {
          GlobalData::GetInstance().cache_.insert(std::pair< size_t, std::pair<char*,size_t*>>(size, std::pair<char*,size_t*>(data_,refCount)));
        } else
        {
          delete[] data_;
          delete refCount;
          GlobalData::GetInstance().totalAllocatedBytes -= size;
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
    GlobalData::GetInstance().totalAllocatedBytes += size;
    GlobalData::GetInstance().numAllocations++;
  }

  char* data() const {return data_;}

  static void EnableCaching()
  {
    GlobalData::GetInstance().cachingEnabled_ = true;
  }

  static void DisableCaching()
  {
    for(auto it=GlobalData::GetInstance().cache_.begin(); it!=GlobalData::GetInstance().cache_.end(); ++it)
    {
      delete[] (*it).second.first;
      delete (*it).second.second;
      GlobalData::GetInstance().totalAllocatedBytes -= (*it).first;
    }
    GlobalData::GetInstance().cache_.clear();
    GlobalData::GetInstance().cachingEnabled_ = false;
  }

  friend std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem);

  static size_t numAllocations;

private:
  char* data_ = nullptr;
  size_t size = 0;
  size_t* refCount  = nullptr;
  bool ownData = false;

  /*static size_t totalAllocatedBytes;

  static bool cachingEnabled_;
  static std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;*/
};

/*size_t MemoryBlock::numAllocations = 0;
size_t MemoryBlock::totalAllocatedBytes = 0;
bool MemoryBlock::cachingEnabled_ = false;
std::unordered_multimap<size_t, std::pair<char*,size_t*>> MemoryBlock::cache_;*/

inline std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem)
{
  os << "MemoryBlock:" << std::endl;
  os << "  size = " << mem.size << std::endl;
  os << "  refCount = " << *mem.refCount << std::endl;
  os << "  dataPtr = " << reinterpret_cast<size_t>(mem.data_) << std::endl;
  os << "  totalAllocatedBytes = " << GlobalData::GetInstance().totalAllocatedBytes << std::endl;

  return os;
}

inline void EnableCaching()
{
  MemoryBlock::EnableCaching();
}

inline void DisableCaching()
{
  MemoryBlock::DisableCaching();
}

}

#endif // MemoryBlock_H
