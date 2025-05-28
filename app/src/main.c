#include "src/paths.h"
#include <raylib.h>
#include WINDOW_MANAGER
#include MEMORY_MANAGER
#include SCENE_PROVIDER
#include SCENE_MANAGER
#include MAIN_MENU
#include MUSIC_MANAGER
#include OPTIONS
#define RAYGUI_IMPLEMENTATION
#include RAYGUI
#include GLOBAL_MOUSE
#include GLOBAL_WEBSOCKET_MANAGER

#if defined(PLATFORM_ANDROID)
#include <android_native_app_glue.h>
#include ANDROID_BRIDGE
#endif

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#endif

MemoryManager *memoryManager = NULL;
SceneProvider *sceneProvider = NULL;
GlobalGameOptions *globalGameOptions = NULL;
GlobalMouse *globalMouse = NULL;
SceneManager *sceneManager = NULL;
MusicManager *musicManager = NULL;
GlobalWebsocketManager *globalWebsocketManager = NULL;
#if defined(PLATFORM_ANDROID)
AndroidBridge *androidBridge = NULL;
#endif

bool windowShouldClose = false;

void runSceneWrapper(void) {
    musicManager->updateMusic(musicManager);
    sceneManager->runScene(sceneProvider);
}

int main(void) {
#if defined(PLATFORM_DESKTOP)
    ChangeDirectory(TextFormat("%s", GetApplicationDirectory()));
#endif
    TraceLog(LOG_INFO, "Starting application initialization");
    createWindow();
    InitAudioDevice();

    TraceLog(LOG_INFO, "Creating managers");
    memoryManager = createMemoryManager();
    sceneProvider = createSceneProvider();
    globalGameOptions = createGlobalGameOptions();
    globalMouse = createGlobalMouse();
    sceneManager = createSceneManager();
    globalWebsocketManager = createGlobalWebsocketManager();
#if defined(PLATFORM_ANDROID)
    androidBridge = createAndroidBridge();
#endif

    MainMenu *mainMenu = createMainMenu();
    sceneProvider->setScene((Scene *)mainMenu, sceneProvider);

    TraceLog(LOG_INFO, "Initializing music manager");
    musicManager = createMusicManager(ASSETS_PATH_PREFIX "audio/music.ogg");
    musicManager->playMusic(musicManager);

    TraceLog(LOG_INFO, "Connecting to websocket");
    globalWebsocketManager->connectWebsocket(globalWebsocketManager, ADDRESS, PORT);
    TraceLog(LOG_INFO, "Websocket connection initiated");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(runSceneWrapper, 0, 1);
#else
    TraceLog(LOG_INFO, "Entering main game loop");
    while (!WindowShouldClose() && !windowShouldClose) {
        runSceneWrapper();
    }
    TraceLog(LOG_INFO, "Exiting main game loop");
#endif

    TraceLog(LOG_INFO, "Cleaning up resources");

    musicManager->stopMusic(musicManager);

    globalWebsocketManager->disconnectWebsocket(globalWebsocketManager);

    sceneProvider->scene->destroy(sceneProvider->scene);
    musicManager->destroy(musicManager);

    CloseAudioDevice();

    destroyMemoryManager(memoryManager);
    CloseWindow();

    TraceLog(LOG_INFO, "Application shutdown complete");

    return 0;
}
