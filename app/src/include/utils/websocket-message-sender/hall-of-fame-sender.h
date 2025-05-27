#ifndef HALL_OF_FAME_SENDER_H
#define HALL_OF_FAME_SENDER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include GLOBAL_WEBSOCKET_MANAGER
#include <cjson/cJSON.h>
#include <stdlib.h>

#define FETCH_BEST_PLAYERS_EVENT "fetch-best-players"

typedef struct HallOfFameSender HallOfFameSender;

struct HallOfFameSender {
    void (*fetchBestPlayers)(int page);
};

HallOfFameSender *createHallOfFameSender();

#endif
