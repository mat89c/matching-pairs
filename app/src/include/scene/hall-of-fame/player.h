#ifndef PLAYER_H
#define PLAYER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include <string.h>

#define MAX_PLAYER_NICKNAME_LENGTH 100
#define MAX_PLAYER_DATE_LENGTH 30

typedef struct Player Player;

struct Player {
    int position;
    char nickname[MAX_PLAYER_NICKNAME_LENGTH];
    int time;
    int attempts;
    char date[MAX_PLAYER_DATE_LENGTH];
};

Player *createPlayer(int position, char *nickname, int time, int attempts, char *date);

#endif
