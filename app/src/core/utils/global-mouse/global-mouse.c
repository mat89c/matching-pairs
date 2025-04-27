#include "src/paths.h"
#include GLOBAL_MOUSE
#include SCENE_PROVIDER

Vector2 getMousePosition(void) {
    Vector2 mousePos = GetMousePosition();
    mousePos.x = (mousePos.x - sceneProvider->offsetX) / sceneProvider->windowScale;
    mousePos.y = (mousePos.y - sceneProvider->offsetY) / sceneProvider->windowScale;

    return mousePos;
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
