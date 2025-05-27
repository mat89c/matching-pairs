#ifndef HALL_OF_FAME_RECEIVER_H
#define HALL_OF_FAME_RECEIVER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include PLAYER
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FETCH_BEST_PLAYERS_EVENT "fetch-best-players"

typedef struct HallOfFameReceiver HallOfFameReceiver;

struct HallOfFameReceiver {
    bool (*isBestPlayersList)(cJSON *cJSON);
    void (*bestPlayersList)(cJSON *cJSON, Player **players, int *playersCount, int *page, int *totalPlayers,
                            int *totalPages);
};

HallOfFameReceiver *createHallOfFameReceiver();

#endif
