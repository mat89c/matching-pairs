#include "src/paths.h"
#include SCENE_MANAGER

static void runScene(SceneProvider *sceneProvider) {
    Scene *currentScene = (Scene *)sceneProvider->scene;

    if (NULL != sceneProvider->previousScene) {
        sceneProvider->previousScene->destroy(sceneProvider->previousScene);
        memoryManager->freeMemoryAtIndex(sceneProvider->previousSceneMemoryIndex);
        sceneProvider->previousScene = NULL;
    }

    if (false == currentScene->initialized) {
        currentScene->initialized = true;
    }

    currentScene->update(currentScene);

    BeginDrawing();
    ClearBackground(BLACK);

    Camera2D camera = {0};
    camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = (Vector2){WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = fminf((float)GetScreenWidth() / WINDOW_WIDTH, (float)GetScreenHeight() / WINDOW_HEIGHT);
    sceneProvider->camera = camera;

    BeginMode2D(camera);
    currentScene->draw(currentScene);
    EndMode2D();

    EndDrawing();
}

SceneManager *createSceneManager() {
    SceneManager *sceneManager = malloc(sizeof(SceneManager));
    if (sceneManager == NULL) {
        exit(EXIT_FAILURE);
    }

    sceneManager->runScene = runScene;

    memoryManager->addObject(sceneManager);

    return sceneManager;
}
