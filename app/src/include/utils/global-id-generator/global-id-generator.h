#ifndef GLOBAL_ID_GENERATOR_H
#define GLOBAL_ID_GENERATOR_H

#include "src/paths.h"
#include MEMORY_MANAGER

typedef struct GlobalIdGenerator GlobalIdGenerator;

struct GlobalIdGenerator {
    int id;
    int (*generateId)(GlobalIdGenerator *self);
};

GlobalIdGenerator *createGlobalIdGenerator(void);

extern GlobalIdGenerator *globalIdGenerator;

#endif
