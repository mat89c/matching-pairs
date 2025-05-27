#ifndef HALL_OF_FAME_H
#define HALL_OF_FAME_H

#include "src/paths.h"
#include SCENE
#include SCENE_PROVIDER
#include MEMORY_MANAGER
#include SOUND_MANAGER
#include GLOBAL_MOUSE
#include MAIN_MENU
#include HALL_OF_FAME_RECEIVER
#include HALL_OF_FAME_SENDER
#include PLAYER
#include WEBSOCKET_SUBSCRIBER

#define MAX_PLAYERS 10

typedef struct HallOfFame HallOfFame;

struct HallOfFame {
    Scene base;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    Texture2D background;
    Texture2D returnBtnTexture;
    Rectangle returnBtnRect;
    SoundManager *clickSound;
    WebsocketSubscriber *websocketSubscriber;
    HallOfFameReceiver *hallOfFameMessageReceiver;
    HallOfFameSender *hallOfFameMessageSender;
    int currentPage;
    int totalPlayers;
    int totalPages;
    Player **players;
    int playersCount;
    Texture2D headerTexture;
    Texture2D nextBtnTexture;
    Texture2D prevBtnTexture;
    Rectangle nextBtnRect;
    Rectangle prevBtnRect;
};

HallOfFame *createHallOfFame();

#endif
