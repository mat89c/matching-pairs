#ifndef MATCHING_PAIRS_MESSAGE_SENDER_H
#define MATCHING_PAIRS_MESSAGE_SENDER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include GLOBAL_WEBSOCKET_MANAGER
#include <cjson/cJSON.h>
#include <stdbool.h>

#define GAME_START "game-start"
#define NICKNAME_SUBMIT "nickname-submit"

typedef struct MatchingPairsMessageSender {
    void (*startGame)(void);
    void (*submitNickname)(char *nickname);
} MatchingPairsMessageSender;

MatchingPairsMessageSender *createMatchingPairsMessageSender();

#endif
