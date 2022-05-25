#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define TOTAL_HEAP_SIZE 209715200 // 200MB
#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x07

typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT createMemManager(void *const memoryForMemoryManager, void *const managedMemory);

void *memoryAlloc(MemoryManagerADT const memoryManager, unsigned int memoryToAllocate);

void memoryFree(MemoryManagerADT const memoryManager, void *memoryToFree);

unsigned int heapSize();

unsigned int heapLeft(MemoryManagerADT mm);

unsigned int usedHeap(MemoryManagerADT mm);

#endif