#ifndef GLOBAL_GAME_OPTIONS_H
#define GLOBAL_GAME_OPTIONS_H
#include "src/paths.h"
#include MEMORY_MANAGER

typedef struct GlobalGameOptions GlobalGameOptions;

struct GlobalGameOptions {
    float musicVolume;
    float sfxVolume;
};

GlobalGameOptions *createGlobalGameOptions(void);

extern GlobalGameOptions *globalGameOptions;

#endif
