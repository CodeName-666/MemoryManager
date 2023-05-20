#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include <vector>
#include "../MemoryManager_Types.h"
#include "LinkedList.h"

using MemoryManagerBlockList = std::vector<MemoryManagerBlock>;
using MemoryManagerBlockParamList = std::vector<MemoryBlockParams_t*>;


#endif