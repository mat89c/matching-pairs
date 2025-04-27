#include "src/paths.h"
#include SCENE_MANAGER

SceneManager *createSceneManager() {
    SceneManager *sceneManager = malloc(sizeof(SceneManager));
    if (sceneManager == NULL) {
        exit(EXIT_FAILURE);
    }

    sceneManager->runScene = runScene;
    sceneManager->renderTarget = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    sceneManager->windowScale = 1.0f;
    sceneManager->renderWidth = WINDOW_WIDTH;
    sceneManager->renderHeight = WINDOW_HEIGHT;
    sceneManager->offsetX = 0;
    sceneManager->offsetY = 0;

    SetTextureFilter(sceneManager->renderTarget.texture, TEXTURE_FILTER_BILINEAR);

    memoryManager->addObject(sceneManager);

    return sceneManager;
}

void runScene(SceneProvider *sceneProvider) {
    Scene *currentScene = (Scene *)sceneProvider->scene;

    if (false == currentScene->initialized) {
        currentScene->initialized = true;

        if (NULL != sceneProvider->previousScene) {
            sceneProvider->previousScene->destroy(sceneProvider->previousScene);
            memoryManager->freeMemoryAtIndex(sceneProvider->previousSceneMemoryIndex);
        }
    }

    float scale = fminf((float)GetScreenWidth() / WINDOW_WIDTH, (float)GetScreenHeight() / WINDOW_HEIGHT);
    sceneProvider->windowScale = scale;
    sceneProvider->renderWidth = WINDOW_WIDTH * scale;
    sceneProvider->renderHeight = WINDOW_HEIGHT * scale;
    sceneProvider->offsetX = (GetScreenWidth() - sceneProvider->renderWidth) / 2;
    sceneProvider->offsetY = (GetScreenHeight() - sceneProvider->renderHeight) / 2;

    currentScene->update(currentScene);
    currentScene->draw(currentScene);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(sceneProvider->renderTarget.texture,
                   (Rectangle){0, 0, (float)sceneProvider->renderTarget.texture.width,
                               -(float)sceneProvider->renderTarget.texture.height},
                   (Rectangle){sceneProvider->offsetX, sceneProvider->offsetY, sceneProvider->renderWidth,
                               sceneProvider->renderHeight},
                   (Vector2){0, 0}, 0.0f, WHITE);

    EndDrawing();
}
