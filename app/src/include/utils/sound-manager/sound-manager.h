#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <raylib.h>

typedef struct SoundManager SoundManager;

struct SoundManager {
    Sound sound;
    void (*playSound)(SoundManager *soundManager);
    void (*destroy)(SoundManager *soundManager);
};

SoundManager *createSoundManager(char *soundPath);

#endif
