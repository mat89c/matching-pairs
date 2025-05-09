#include "src/paths.h"
#include MEMORY_MANAGER

static void addObject(void *object) {
    size_t newCount = memoryManager->count + 1;
    void **newObjects = realloc(memoryManager->objects, newCount * sizeof(void *));

    if (NULL == newObjects) {
        exit(EXIT_FAILURE);
    }

    memoryManager->objects = newObjects;
    memoryManager->objects[newCount - 1] = object;
    memoryManager->count = newCount;
}

static void freeMemoryAtIndex(size_t index) {
    if (NULL == memoryManager->objects[index]) {
        return;
    }

    free(memoryManager->objects[index]);
    memoryManager->objects[index] = NULL;
}

static void freeMemory(MemoryManager *memoryManager) {
    for (size_t i = 0; i < memoryManager->count; i++) {
        if (NULL == memoryManager->objects[i]) {
            continue;
        }

        free(memoryManager->objects[i]);
        memoryManager->objects[i] = NULL;
    }

    free(memoryManager->objects);
    memoryManager->objects = NULL;
    memoryManager->count = 0;
}

void destroyMemoryManager(MemoryManager *memoryManager) {
    freeMemory(memoryManager);
    free(memoryManager);
}

static size_t nextIndex(MemoryManager *memoryManager) { return memoryManager->count - 1; }

MemoryManager *createMemoryManager(void) {
    memoryManager = malloc(sizeof(MemoryManager));
    if (NULL == memoryManager) {
        exit(EXIT_FAILURE);
    }

    memoryManager->objects = NULL;
    memoryManager->count = 0;
    memoryManager->addObject = addObject;
    memoryManager->freeMemoryAtIndex = freeMemoryAtIndex;
    memoryManager->freeMemory = freeMemory;
    memoryManager->nextIndex = nextIndex;

    return memoryManager;
}
