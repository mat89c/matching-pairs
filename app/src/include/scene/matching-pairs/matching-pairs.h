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
#include GLOBAL_WEBSOCKET_MANAGER
#include <cjson/cJSON.h>
#include <raylib.h>
#include <stdbool.h>
#include MATCHING_PAIRS_MESSAGE_SENDER
#include CARD_MESSAGE_SENDER
#include MATCHING_PAIRS_MESSAGE_RECEIVER
#include CARD_MESSAGE_RECEIVER
#include HALL_OF_FAME
#include ANDROID_BRIDGE

#define NUMBER_OF_PAIRS 9
#define NUMBER_OF_CARDS NUMBER_OF_PAIRS * 2
#define NUMBER_OF_ROWS 6
#define NUMBER_OF_COLUMNS 6
#define MATCH_DELAY_TIME 3.0f
#define MAX_NICKNAME_LENGTH 20

typedef struct Card Card;

typedef struct MatchingPairs MatchingPairs;

struct MatchingPairs {
    Scene base;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    bool isCardFlipped;
    char *flippedCardIndex;
    Card *cards[NUMBER_OF_CARDS];
    CardTexture *cardTexture;
    int numberOfAttempts;
    float timer;
    Texture2D backgroundTexture;
    bool isGameFinished;
    Texture2D returnBtnTexture;
    Rectangle returnBtnRect;
    SoundManager *clickSound;
    SoundManager *matchSound;
    WebsocketSubscriber *websocketSubscriber;
    char *cardIndices[NUMBER_OF_CARDS];
    void (*createCards)(MatchingPairs *matchingPairs);
    bool isDataLoaded;
    MatchingPairsMessageSender *matchingPairsMessageSender;
    CardMessageSender *cardMessageSender;
    MatchingPairsMessageReceiver *matchingPairsMessageReceiver;
    CardMessageReceiver *cardMessageReceiver;
    Texture2D youWonBackgroundTexture;
    Texture2D submitBtnTexture;
    Texture2D submitBtnPressedTexture;
    Rectangle submitBtnRect;
    bool isSubmitBtnHovered;
    char nickname[MAX_NICKNAME_LENGTH];
    Rectangle nicknameRect;
    bool isNicknameFocused;
    bool isNicknameSubmitted;
    char *activeTextBox;
    int activeTextBoxMaxLength;
};

MatchingPairs *createMatchingPairs();

#endif
