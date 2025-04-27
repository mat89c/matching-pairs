#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

typedef struct MemoryManager MemoryManager;

typedef void (*ObjectAdder)(void *object);

struct MemoryManager {
    void **objects;
    size_t count;
    void (*freeMemoryAtIndex)(size_t index);
    void (*freeMemory)(MemoryManager *memoryManager);
    size_t (*nextIndex)(MemoryManager *memoryManager);
    ObjectAdder addObject;
};

void destroyMemoryManager(MemoryManager *memoryManager);

MemoryManager *createMemoryManager(void);

extern MemoryManager *memoryManager;

#endif
