// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/the_memory_manager.h"
#include "../include/mem_manager_ADT.h"

static MemoryManagerADT manager;

void init_mem_manager(void *const restrict mem_for_manager, void *const restrict managedMemory) {
  manager = createMemManager(mem_for_manager, managedMemory);
} // hay que inicializarlo en el arranque del kernel

void *allocMemory(unsigned int to_alloc) {
  return memoryAlloc(manager, to_alloc);
}

void freeMemory(void *to_free) {
    // para despues
    memoryFree(manager, to_free);
}
