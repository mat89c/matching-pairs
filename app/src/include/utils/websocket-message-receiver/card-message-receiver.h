#ifndef CARD_MESSAGE_RECEIVER_H
#define CARD_MESSAGE_RECEIVER_H

#include "src/paths.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <string.h>
#include MEMORY_MANAGER
#include CARD_TEXTURE
#include <raylib.h>

#define CARD_FLIPPED "card-flipped"
#define CARDS_MATCH "cards-match"
#define HIDE_CARDS "hide-cards"

typedef struct Card Card;

typedef struct CardMessageReceiver {
    bool (*isCardFlipped)(cJSON *json);
    void (*flipCard)(cJSON *json, Card **cards, CardTexture *cardTexture);
    bool (*isCardsMatch)(cJSON *json);
    void (*matchCards)(cJSON *json, Card **cards);
    bool (*isHideCards)(cJSON *json);
    void (*hideCards)(cJSON *json, Card **cards, int *numberOfAttempts);
} CardMessageReceiver;

CardMessageReceiver *createCardMessageReceiver();

#endif
