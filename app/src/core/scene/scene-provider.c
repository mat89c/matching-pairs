#include "src/paths.h"
#include SCENE_PROVIDER

static void setScene(Scene *newScene, SceneProvider *self) {
    if (NULL != self->scene) {
        self->previousScene = self->scene;
        self->previousSceneMemoryIndex = self->scene->memoryManagerIndex;
    }

    self->scene = newScene;
}

SceneProvider *createSceneProvider(void) {
    SceneProvider *sceneProvider = malloc(sizeof(SceneProvider));
    if (NULL == sceneProvider) {
        exit(EXIT_FAILURE);
    }

    sceneProvider->scene = NULL;
    sceneProvider->previousScene = NULL;
    sceneProvider->setScene = setScene;
    sceneProvider->previousSceneMemoryIndex = -1;
    sceneProvider->camera = (Camera2D){0};

    memoryManager->addObject(sceneProvider);

    return sceneProvider;
}
