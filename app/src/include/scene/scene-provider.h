#ifndef SCENE_PROVIDER_H
#define SCENE_PROVIDER_H

#include "src/paths.h"
#include <raylib.h>
#include <stdlib.h>
#include MEMORY_MANAGER
#include WINDOW_MANAGER
#include SCENE

typedef struct Scene Scene;

typedef struct SceneProvider SceneProvider;

struct SceneProvider {
    Scene *scene;
    Scene *previousScene;
    size_t previousSceneMemoryIndex;
    void (*setScene)(Scene *scene, SceneProvider *self);
    Camera2D camera;
};

SceneProvider *createSceneProvider();

extern SceneProvider *sceneProvider;

#endif
