#ifndef MATCHING_PAIRS_MESSAGE_RECEIVER_H
#define MATCHING_PAIRS_MESSAGE_RECEIVER_H

#include "src/paths.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <string.h>
#include MEMORY_MANAGER

#define GAME_STARTED "game-started"
#define GAME_FINISHED "game-finished"
#define NICKNAME_SUBMITTED "nickname-submitted"

typedef struct MatchingPairsMessageReceiver {
    bool (*isGameStarted)(cJSON *json);
    void (*initCardIndices)(cJSON *json, char **cardIndices);
    bool (*isGameFinished)(cJSON *json);
    void (*gameFinished)(cJSON *json, bool *isGameFinished, float *timer);
    bool (*isNicknameSubmitted)(cJSON *json);
} MatchingPairsMessageReceiver;

MatchingPairsMessageReceiver *createMatchingPairsMessageReceiver();

#endif
