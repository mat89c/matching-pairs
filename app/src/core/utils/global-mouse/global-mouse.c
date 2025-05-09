#include "src/paths.h"
#include GLOBAL_MOUSE
#include SCENE_PROVIDER

static Vector2 getMousePosition(void) {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), sceneProvider->camera);

#if defined(PLATFORM_WEB)
    float scaleX = (float)GetScreenWidth() / WINDOW_WIDTH;
    float scaleY = (float)GetScreenHeight() / WINDOW_HEIGHT;

    Vector2 mouseWorldPosScaled = {mouseWorldPos.x / scaleX, mouseWorldPos.y / scaleY};

    return mouseWorldPosScaled;
#endif

    return mouseWorldPos;
}

GlobalMouse *createGlobalMouse(void) {
    GlobalMouse *globalMouse = malloc(sizeof(GlobalMouse));
    if (NULL == globalMouse) {
        exit(EXIT_FAILURE);
    }

    globalMouse->getMousePosition = getMousePosition;

    memoryManager->addObject(globalMouse);
    return globalMouse;
}
