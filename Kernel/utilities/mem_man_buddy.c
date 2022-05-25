// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY
#include <buddyADT.h>
#include "../include/naiveConsole.h"
#include <mem_manager_ADT.h>
#include <stdint.h>

#define BLOCK_SIZE_MIN 512
#define NULL ((void *) 0)

typedef unsigned int size_t;

typedef struct MemoryBlock
{
  unsigned char isFree;
  size_t history;
    size_t blockSize;   //El espacio neto no sera potencia de 2, porque incluye al header, pero el total si
    struct MemoryBlock *pnextFreeBlock;
} MemoryBlock;

typedef struct MemoryManagerCDT {
  MemoryBlock start;
  MemoryBlock end;
  size_t freeBytesRemaining;
} MemoryManagerCDT;

static const uint16_t STRUCT_SIZE = ((sizeof(MemoryBlock) + (BYTE_ALIGNMENT - 1)) & ~BYTE_ALIGNMENT_MASK);

MemoryManagerADT createMemManager(void *const restrict MemoryManagerMem, void *const restrict managedMemory) {
  MemoryManagerADT memoryManager = (MemoryManagerADT) MemoryManagerMem;
  memoryManager->freeBytesRemaining = TOTAL_HEAP_SIZE;

  MemoryBlock *firstFreeBlock = (void *) managedMemory;

	memoryManager->start.pnextFreeBlock = (void *) firstFreeBlock;
	memoryManager->start.blockSize = (size_t) 0;

	memoryManager->end.pnextFreeBlock = NULL;
	memoryManager->end.blockSize = TOTAL_HEAP_SIZE;

	firstFreeBlock->blockSize = TOTAL_HEAP_SIZE;
	firstFreeBlock->pnextFreeBlock = &memoryManager->end;
    firstFreeBlock->isFree = 1;
    firstFreeBlock->history = 0;

	return memoryManager;
}

static void removeBlockFromFreeList(MemoryManagerADT memoryManager, MemoryBlock *blockToRemove) {
  MemoryBlock *blockIter = &memoryManager->start;
            
  while (blockIter != NULL && blockIter->pnextFreeBlock != blockToRemove)
    blockIter = blockIter->pnextFreeBlock;

  if (blockIter) {
    blockIter->pnextFreeBlock = blockIter->pnextFreeBlock->pnextFreeBlock;
  }
}

static size_t insertBlockIntoFreeList(MemoryManagerADT memoryManager, MemoryBlock *blockToInsert, uint8_t checkIfMerge) {
  if (blockToInsert->blockSize == TOTAL_HEAP_SIZE) {
    memoryManager->start.pnextFreeBlock = blockToInsert;
    blockToInsert->pnextFreeBlock = &memoryManager->end;
    blockToInsert->isFree = 1;
    return blockToInsert->blockSize;
  }

  MemoryBlock *buddy = NULL;
  int merge = 0;

  if (checkIfMerge) {
    if ((blockToInsert->history & 0x1) == 1) {
      buddy = (MemoryBlock *) ((uint64_t) blockToInsert - blockToInsert->blockSize);
      if (buddy->isFree && buddy->blockSize == blockToInsert->blockSize) {
        buddy->blockSize *= 2;
        blockToInsert = buddy;
        merge = 1;
      }
    }
    else {
      buddy = (MemoryBlock *) ((uint64_t) blockToInsert + blockToInsert->blockSize);
      if (buddy->isFree && buddy->blockSize == blockToInsert->blockSize) {
        blockToInsert->blockSize *= 2;
        merge = 1;
      }
    }
  }

  if (merge) {
    blockToInsert->history = blockToInsert->history >> 1;
    removeBlockFromFreeList(memoryManager, buddy);
    // Se checkea si es posible seguir mergeando
    return insertBlockIntoFreeList(memoryManager, blockToInsert, 1);
  }

  MemoryBlock *blockIter = &memoryManager->start;
  size_t blockSize = blockToInsert->blockSize;
            
  while (blockIter->pnextFreeBlock->blockSize < blockSize)
    blockIter = blockIter->pnextFreeBlock;
              
  blockToInsert->pnextFreeBlock = blockIter->pnextFreeBlock;
  blockIter->pnextFreeBlock = blockToInsert;
  blockToInsert->isFree = 1;
  return blockToInsert->blockSize;
}

void *memoryAlloc(MemoryManagerADT const memoryManager, size_t memoryToAlloc) {
  MemoryBlock *block, *prevBlock;
  void *blockToReturn = NULL;

  if (memoryToAlloc == 0)
    return NULL;

  memoryToAlloc += STRUCT_SIZE;

  if ((memoryToAlloc & BYTE_ALIGNMENT_MASK) != 0) {
    memoryToAlloc += (BYTE_ALIGNMENT - (memoryToAlloc & BYTE_ALIGNMENT_MASK));
  }

  if (memoryToAlloc > TOTAL_HEAP_SIZE) {
    return NULL;
  }
  
  prevBlock = &memoryManager->start;
  block = memoryManager->start.pnextFreeBlock;

  while ((block->blockSize < memoryToAlloc) && (block->pnextFreeBlock != NULL)) {
    prevBlock = block;
    block = block->pnextFreeBlock;
  }

  if (block == &memoryManager->end)
    return NULL;

  blockToReturn = (void *) (((uint8_t *) prevBlock->pnextFreeBlock) + STRUCT_SIZE);

  prevBlock->pnextFreeBlock = block->pnextFreeBlock;

  while (block->blockSize / 2 >= BLOCK_SIZE_MIN && block->blockSize / 2 >= memoryToAlloc) {
    block->blockSize /= 2;
    block->history = block->history << 1;
    MemoryBlock *newBlock = (void *) (((uint8_t *) block) + block->blockSize);
    newBlock->blockSize = block->blockSize;
    newBlock->isFree = 1;
    newBlock->history = block->history | 0x1;
    insertBlockIntoFreeList(memoryManager, newBlock, 0);
  }

  memoryManager->freeBytesRemaining -= block->blockSize;
  block->isFree = 0;
  

	return blockToReturn;
}

void memoryFree(MemoryManagerADT const memoryManager, void *block) {
  if (block == NULL)
    return;

  uint8_t *memoryToFree = (uint8_t *) block;
  MemoryBlock *blockToFree;

  memoryToFree -= STRUCT_SIZE;
  blockToFree = (void *) memoryToFree;


  size_t freedMemory = insertBlockIntoFreeList(memoryManager, ((MemoryBlock *) blockToFree), 1);
  memoryManager->freeBytesRemaining += freedMemory;

}

size_t heapSize() {
  return TOTAL_HEAP_SIZE;
}

size_t heapLeft(MemoryManagerADT mm) {
  return mm->freeBytesRemaining;
}

size_t usedHeap(MemoryManagerADT mm) {
  return heapSize() - heapLeft(mm);
}
#endif