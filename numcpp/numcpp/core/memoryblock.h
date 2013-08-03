#ifndef NUMCPP_MEMORYBLOCK_H
#define NUMCPP_MEMORYBLOCK_H

#include <cstdlib>
#include <iosfwd>
#include <unordered_map>

extern template class std::pair<char*,size_t*>;
extern template class std::unordered_multimap<size_t, std::pair<char*,size_t*> >;

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

  /*static size_t totalAllocatedBytes;

  static bool cachingEnabled_;
  static std::unordered_multimap<size_t, std::pair<char*,size_t*>> cache_;*/
};

/*size_t MemoryBlock::numAllocations = 0;
size_t MemoryBlock::totalAllocatedBytes = 0;
bool MemoryBlock::cachingEnabled_ = false;
std::unordered_multimap<size_t, std::pair<char*,size_t*>> MemoryBlock::cache_;*/



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
