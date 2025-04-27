#include "src/paths.h"
#include SOUND_MANAGER
#include MEMORY_MANAGER

static void playSound(SoundManager *soundManager) { PlaySound(soundManager->sound); }

static void destroy(SoundManager *soundManager) { UnloadSound(soundManager->sound); }

SoundManager *createSoundManager(char *soundPath) {
    SoundManager *soundManager = malloc(sizeof(SoundManager));

    if (NULL == soundManager) {
        exit(EXIT_FAILURE);
    }

    soundManager->sound = LoadSound(soundPath);
    soundManager->playSound = playSound;
    soundManager->destroy = destroy;

    memoryManager->addObject(soundManager);

    return soundManager;
}
