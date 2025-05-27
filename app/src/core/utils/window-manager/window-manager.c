#include "src/paths.h"
#include WINDOW_MANAGER

void createWindow(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Matching Pairs");
    SetTargetFPS(FPS);

#if defined(PLATFORM_WEB)
    SetWindowMaxSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
#endif
}
