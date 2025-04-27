#include "src/paths.h"
#include WINDOW_MANAGER

bool windowShouldClose = false;

void createWindow(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Matching Pairs");
    SetTargetFPS(FPS);
}
