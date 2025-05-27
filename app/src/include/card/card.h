#ifndef CARD_H
#define CARD_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include CARD_TEXTURE
#include GLOBAL_MOUSE
#include SOUND_MANAGER
#include MATCHING_PAIRS
#include <cjson/cJSON.h>
#include <raylib.h>
#include <stdbool.h>

#define CARD_WIDTH 161
#define CARD_HEIGHT 161
#define SPACING 10

struct MatchingPairs;

typedef struct Card Card;

struct Card {
    Rectangle destination;
    char *index;
    bool isFlipped;
    Texture2D frontTexture;
    Texture2D backTexture;
    void (*destroy)(Card *card);
    void (*draw)(Card *card);
    void (*update)(Card *card, struct MatchingPairs *matchingPairs);
    void (*onFlip)(Card *card, struct MatchingPairs *matchingPairs);
    bool isVisible;
    SoundManager *clickSound;
};

Card *createCard(CardTexture *cardTexture, char *index);

#endif
