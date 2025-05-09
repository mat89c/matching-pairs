#include "src/paths.h"
#include GLOBAL_ID_GENERATOR

static int generateId(GlobalIdGenerator *self) { return self->id++; }

GlobalIdGenerator *createGlobalIdGenerator(void) {
    GlobalIdGenerator *globalIdGenerator = malloc(sizeof(GlobalIdGenerator));

    if (NULL == globalIdGenerator) {
        exit(EXIT_FAILURE);
    }

    globalIdGenerator->id = 0;
    globalIdGenerator->generateId = generateId;

    memoryManager->addObject(globalIdGenerator);

    return globalIdGenerator;
}
