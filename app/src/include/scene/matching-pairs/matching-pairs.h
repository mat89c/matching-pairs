#ifndef MATCHING_PAIRS_H
#define MATCHING_PAIRS_H

#include "src/paths.h"
#include SCENE
#include MEMORY_MANAGER
#include CARD
#include SCENE_PROVIDER
#include CARD_TEXTURE
#include SOUND_MANAGER
#include MAIN_MENU
#include <raylib.h>
#include <stdbool.h>

#define NUMBER_OF_PAIRS 9
#define NUMBER_OF_CARDS NUMBER_OF_PAIRS * 2
#define NUMBER_OF_ROWS 6
#define NUMBER_OF_COLUMNS 6
#define MATCH_DELAY_TIME 3.0f
typedef struct MatchingPairs MatchingPairs;

struct MatchingPairs {
    Scene base;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    Card *cards[NUMBER_OF_CARDS];
    CardTexture *cardTexture;
    Card *firstFlippedCard;
    Card *secondFlippedCard;
    int numberOfAttempts;
    int numberOfMatches;
    bool awaitingForMatch;
    float matchDelayTimer;
    float timer;
    Texture2D backgroundTexture;
    bool isFinished;
    Texture2D returnBtnTexture;
    Rectangle returnBtnRect;
    SoundManager *clickSound;
    SoundManager *matchSound;
};

MatchingPairs *createMatchingPairs();

#endif
