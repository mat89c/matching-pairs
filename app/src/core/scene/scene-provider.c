#include "src/paths.h"
#include SCENE_PROVIDER

void setScene(Scene *newScene, SceneProvider *self) {
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
    sceneProvider->renderTarget = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    sceneProvider->windowScale = 1.0f;
    sceneProvider->renderWidth = WINDOW_WIDTH;
    sceneProvider->renderHeight = WINDOW_HEIGHT;
    sceneProvider->offsetX = 0;
    sceneProvider->offsetY = 0;

    memoryManager->addObject(sceneProvider);

    return sceneProvider;
}
