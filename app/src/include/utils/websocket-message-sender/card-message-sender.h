#ifndef CARD_MESSAGE_SENDER_H
#define CARD_MESSAGE_SENDER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include GLOBAL_WEBSOCKET_MANAGER
#include <cjson/cJSON.h>
#include <stdbool.h>

#define CARD_FLIP "card-flip"

typedef struct CardMessageSender {
    void (*flip)(char *cardIndex);
} CardMessageSender;

CardMessageSender *createCardMessageSender();

#endif
