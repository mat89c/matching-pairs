#include "src/paths.h"
#include MUSIC_MANAGER
#include MEMORY_MANAGER

static void playMusic(MusicManager *musicManager) { PlayMusicStream(musicManager->music); }

static void updateMusic(MusicManager *musicManager) { UpdateMusicStream(musicManager->music); }

static void stopMusic(MusicManager *musicManager) { StopMusicStream(musicManager->music); }

static void destroy(MusicManager *musicManager) { UnloadMusicStream(musicManager->music); }

MusicManager *createMusicManager(char *musicPath) {
    MusicManager *musicManager = malloc(sizeof(MusicManager));

    if (NULL == musicManager) {
        exit(EXIT_FAILURE);
    }

    musicManager->music = LoadMusicStream(musicPath);
    musicManager->playMusic = playMusic;
    musicManager->stopMusic = stopMusic;
    musicManager->updateMusic = updateMusic;
    musicManager->destroy = destroy;

    memoryManager->addObject(musicManager);

    return musicManager;
}
