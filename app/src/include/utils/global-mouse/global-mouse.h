#ifndef GLOBAL_MOUSE_H
#define GLOBAL_MOUSE_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include SCENE_PROVIDER
#include "raylib.h"

typedef struct GlobalMouse GlobalMouse;

struct GlobalMouse {
    Vector2 (*getMousePosition)(void);
};

GlobalMouse *createGlobalMouse(void);

extern GlobalMouse *globalMouse;

#endif
