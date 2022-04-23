void init_mem_manager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *alloc_mem(unsigned int to_alloc);

void free_mem(void *to_free);

