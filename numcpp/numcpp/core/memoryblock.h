#ifndef NUMCPP_MEMORYBLOCK_H
#define NUMCPP_MEMORYBLOCK_H

#include <iosfwd>

namespace numcpp
{

class AbstractGlobalData
{
public:
  virtual size_t getTotalAllocatedBytes() = 0;
  virtual bool getCachingEnabled() = 0;
  virtual void setCachingEnabled(bool caching) = 0;
};

static AbstractGlobalData& GetGlobalData();

class MemoryBlock
{
public:

  MemoryBlock();
  MemoryBlock(size_t size);
  MemoryBlock(char* data, size_t size, bool ownData=false);
  MemoryBlock(const MemoryBlock& rhs);
  ~MemoryBlock();

  void allocate(size_t size);
  void free();
  char* data() const { return data_; }
  bool isAllocated() { return refCount != nullptr; }

  static void EnableCaching();
  static void DisableCaching();

  friend std::ostream& operator<< (std::ostream& os, const MemoryBlock& mem);

  static size_t numAllocations;
private:
  char* data_ = nullptr;
  size_t size = 0;
  size_t* refCount  = nullptr;
  bool ownData = false;
};



inline void EnableCaching()
{
  MemoryBlock::EnableCaching();
}

inline void DisableCaching()
{
  MemoryBlock::DisableCaching();
}

}

#endif
