#include "memoryblock.h"

template class std::pair<char*,size_t*>;
template class std::unordered_multimap<size_t, std::pair<char*,size_t*> >;

namespace numcpp
{

GlobalData &GlobalData::GetInstance()
{
   static GlobalData instance;
   return instance;
}


MemoryBlock::MemoryBlock()
{
}

MemoryBlock::MemoryBlock(size_t size)
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

MemoryBlock::MemoryBlock(char* data, size_t size, bool ownData)
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
    GlobalData::GetInstance().totalAllocatedBytes += size;
    GlobalData::GetInstance().numAllocations++;
}

void MemoryBlock::free()
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

void MemoryBlock::EnableCaching()
  {
    GlobalData::GetInstance().cachingEnabled_ = true;
  }

void MemoryBlock::DisableCaching()
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

std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem)
{
  os << "MemoryBlock:" << std::endl;
  os << "  size = " << mem.size << std::endl;
  os << "  refCount = " << *mem.refCount << std::endl;
  os << "  dataPtr = " << reinterpret_cast<size_t>(mem.data_) << std::endl;
  os << "  totalAllocatedBytes = " << GlobalData::GetInstance().totalAllocatedBytes << std::endl;

  return os;
}


}
