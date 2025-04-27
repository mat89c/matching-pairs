#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <raylib.h>
#include <stdlib.h>

typedef struct MusicManager MusicManager;

struct MusicManager {
    Music music;
    void (*playMusic)(MusicManager *musicManager);
    void (*updateMusic)(MusicManager *musicManager);
    void (*stopMusic)(MusicManager *musicManager);
    void (*destroy)(MusicManager *musicManager);
};

MusicManager *createMusicManager(char *musicPath);

extern MusicManager *musicManager;

#endif
