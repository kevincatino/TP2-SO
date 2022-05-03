
// #include <buddyADT.h>
// #include "../include/naiveConsole.h"
// #include <mem_manager_ADT.h>
// #include <stdint.h>

// #define MINIMUM_BLOCK_SIZE 512
// #define NULL ((void *) 0)

// typedef unsigned int size_t;

// typedef struct MemoryBlock
// {
//   unsigned char free;
//   size_t history;
// 	size_t blockSize; // incluye el header, por lo que el espacio neto no sera potencia de 2 pero si el total
// 	struct MemoryBlock *nextFreeBlock;
// } MemoryBlock;

// typedef struct MemoryManagerCDT {
//   MemoryBlock start;
//   MemoryBlock end;
//   size_t freeBytesRemaining;
// } MemoryManagerCDT;

// static const uint16_t STRUCT_SIZE = ((sizeof(MemoryBlock) + (BYTE_ALIGNMENT - 1)) & ~BYTE_ALIGNMENT_MASK);

// MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
//   MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
//   memoryManager->freeBytesRemaining = TOTAL_HEAP_SIZE;

//   MemoryBlock *firstFreeBlock = (void *) managedMemory;

// 	memoryManager->start.nextFreeBlock = (void *) firstFreeBlock;
// 	memoryManager->start.blockSize = (size_t) 0;

// 	memoryManager->end.nextFreeBlock = NULL;
// 	memoryManager->end.blockSize = TOTAL_HEAP_SIZE;

// 	firstFreeBlock->blockSize = TOTAL_HEAP_SIZE;
// 	firstFreeBlock->nextFreeBlock = &memoryManager->end;
//     firstFreeBlock->free = 1;
//     firstFreeBlock->history = 0;

// 	return memoryManager;
// }

// static void removeBlockFromFreeList(MemoryManagerADT memoryManager, MemoryBlock *blockToRemove) {
//   MemoryBlock *blockIterator = &memoryManager->start;
            
//   while (blockIterator != NULL && blockIterator->nextFreeBlock != blockToRemove)
//     blockIterator = blockIterator->nextFreeBlock;

//   if (blockIterator) {
//     blockIterator->nextFreeBlock = blockIterator->nextFreeBlock->nextFreeBlock;
//   }
// }

// static size_t insertBlockIntoFreeList(MemoryManagerADT memoryManager, MemoryBlock *blockToInsert, uint8_t checkMerge) {
//   if (blockToInsert->blockSize == TOTAL_HEAP_SIZE) {
//     memoryManager->start.nextFreeBlock = blockToInsert;
//     blockToInsert->nextFreeBlock = &memoryManager->end;
//     blockToInsert->free = 1;
//     return blockToInsert->blockSize;
//   }

//   MemoryBlock *buddy = NULL;
//   int merge = 0;

//   if (checkMerge) {
//     if ((blockToInsert->history & 0x1) == 1) {
//       buddy = (MemoryBlock *) ((uint64_t) blockToInsert - blockToInsert->blockSize);
//       if (buddy->free && buddy->blockSize == blockToInsert->blockSize) {
//         buddy->blockSize *= 2;
//         blockToInsert = buddy;
//         merge = 1;
//       }
//     }
//     else {
//       buddy = (MemoryBlock *) ((uint64_t) blockToInsert + blockToInsert->blockSize);
//       if (buddy->free && buddy->blockSize == blockToInsert->blockSize) {
//         blockToInsert->blockSize *= 2;
//         merge = 1;
//       }
//     }
//   }

//   if (merge) {
//     blockToInsert->history = blockToInsert->history >> 1;
//     removeBlockFromFreeList(memoryManager, buddy);
//     // checkear si es posible seguir mergeando
//     return insertBlockIntoFreeList(memoryManager, blockToInsert, 1);
//   }

//   MemoryBlock *blockIterator = &memoryManager->start;
//   size_t blockSize = blockToInsert->blockSize;
            
//   while (blockIterator->nextFreeBlock->blockSize < blockSize)
//     blockIterator = blockIterator->nextFreeBlock;
              
//   blockToInsert->nextFreeBlock = blockIterator->nextFreeBlock;
//   blockIterator->nextFreeBlock = blockToInsert;
//   blockToInsert->free = 1;
//   return blockToInsert->blockSize;
// }

// void *allocMem(MemoryManagerADT const memoryManager, size_t memoryToAllocate) {
//   MemoryBlock *block, *previousBlock;
//   void *blockToReturn = NULL;

//   if (memoryToAllocate == 0)
//     return NULL;

// 	// vTaskSuspendAll();

//   memoryToAllocate += STRUCT_SIZE;

//   if ((memoryToAllocate & BYTE_ALIGNMENT_MASK) != 0) {
//     memoryToAllocate += (BYTE_ALIGNMENT - (memoryToAllocate & BYTE_ALIGNMENT_MASK));
//   }

//   if (memoryToAllocate > TOTAL_HEAP_SIZE) {
//     return NULL;
//   }
  
//   previousBlock = &memoryManager->start;
//   block = memoryManager->start.nextFreeBlock;

//   while ((block->blockSize < memoryToAllocate) && (block->nextFreeBlock != NULL)) {
//     previousBlock = block;
//     block = block->nextFreeBlock;
//   }

//   if (block == &memoryManager->end)
//     return NULL;

//   blockToReturn = (void *) (((uint8_t *) previousBlock->nextFreeBlock) + STRUCT_SIZE);

//   previousBlock->nextFreeBlock = block->nextFreeBlock;

//   while (block->blockSize / 2 >= MINIMUM_BLOCK_SIZE && block->blockSize / 2 >= memoryToAllocate) {
//     block->blockSize /= 2;
//     block->history = block->history << 1;
//     MemoryBlock *newBlock = (void *) (((uint8_t *) block) + block->blockSize);
//     newBlock->blockSize = block->blockSize;
//     newBlock->free = 1;
//     newBlock->history = block->history | 0x1;
//     insertBlockIntoFreeList(memoryManager, newBlock, 0);
//   }

//   memoryManager->freeBytesRemaining -= block->blockSize;
//   block->free = 0;
  
// 	// ( void ) xTaskResumeAll();

// 	return blockToReturn;
// }

// void freeMem(MemoryManagerADT const memoryManager, void *block) {
//   if (block == NULL)
//     return;

//   uint8_t *memoryToFree = (uint8_t *) block;
//   MemoryBlock *blockToFree;

//   memoryToFree -= STRUCT_SIZE;
//   blockToFree = (void *) memoryToFree;

//   // vTaskSuspendAll();

//   size_t memoryFreed = insertBlockIntoFreeList(memoryManager, ((MemoryBlock *) blockToFree), 1);
//   memoryManager->freeBytesRemaining += memoryFreed;

//   // ( void ) xTaskResumeAll();
// }

// size_t heapSize() {
//   return TOTAL_HEAP_SIZE;
// }

// size_t heapLeft(MemoryManagerADT mm) {
//   return mm->freeBytesRemaining;
// }

// size_t usedHeap(MemoryManagerADT mm) {
//   return heapSize() - heapLeft(mm);
// }