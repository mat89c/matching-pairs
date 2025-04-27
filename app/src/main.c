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
#include <stdio.h>

MemoryManager *memoryManager = NULL;
SceneProvider *sceneProvider = NULL;
GlobalGameOptions *globalGameOptions = NULL;
GlobalIdGenerator *globalIdGenerator = NULL;
GlobalMouse *globalMouse = NULL;

int main(void) {
    createWindow();
    InitAudioDevice();

    memoryManager = createMemoryManager();
    sceneProvider = createSceneProvider();
    globalGameOptions = createGlobalGameOptions();
    globalIdGenerator = createGlobalIdGenerator();
    globalMouse = createGlobalMouse();
    SceneManager *sceneManager = createSceneManager();

    MainMenu *mainMenu = createMainMenu();
    sceneProvider->setScene((Scene *)mainMenu, sceneProvider);

    MusicManager *musicManager = createMusicManager("app/assets/audio/music.ogg");
    musicManager->playMusic(musicManager);

    while (!WindowShouldClose() && !windowShouldClose) {
        SetMusicVolume(musicManager->music, globalGameOptions->musicVolume / 100.0f);
        musicManager->updateMusic(musicManager);
        sceneManager->runScene(sceneProvider);
    }

    UnloadRenderTexture(sceneManager->renderTarget);
    musicManager->destroy(musicManager);
    sceneProvider->scene->destroy(sceneProvider->scene);
    destroyMemoryManager(memoryManager);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
