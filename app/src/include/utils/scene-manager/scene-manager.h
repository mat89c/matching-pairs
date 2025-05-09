#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include SCENE_PROVIDER
#include SCENE
#include <math.h>
#include <stdbool.h>

typedef struct {
    void (*runScene)(SceneProvider *sceneProvider);
    RenderTexture2D renderTarget;
    float windowScale;
    int renderWidth;
    int renderHeight;
    int offsetX;
    int offsetY;
} SceneManager;

SceneManager *createSceneManager();

#endif
