#ifndef SCENE_H
#define SCENE_H
#include <stdbool.h>
#include <stddef.h>

typedef struct Scene Scene;

struct Scene {
    const char *name;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    size_t memoryManagerIndex;
};

#endif
