#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT create_mem_manager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *mem_alloc(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);

#endif