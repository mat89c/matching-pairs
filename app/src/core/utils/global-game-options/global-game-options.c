#include "src/paths.h"
#include GLOBAL_GAME_OPTIONS

GlobalGameOptions *createGlobalGameOptions(void) {
    GlobalGameOptions *globalGameOptions = malloc(sizeof(GlobalGameOptions));
    if (NULL == globalGameOptions) {
        exit(EXIT_FAILURE);
    }

    globalGameOptions->musicVolume = 50.0f;
    globalGameOptions->sfxVolume = 50.0f;

    memoryManager->addObject(globalGameOptions);

    return globalGameOptions;
}
