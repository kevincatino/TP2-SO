void init_mem_manager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory(unsigned int to_alloc);

void freeMemory(void *to_free);
void printMem();

