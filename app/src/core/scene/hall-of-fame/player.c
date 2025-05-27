#include "src/paths.h"
#include PLAYER

Player *createPlayer(int position, char *nickname, int time, int attempts, char *date) {
    Player *player = malloc(sizeof(Player));
    if (NULL == player) {
        exit(EXIT_FAILURE);
    }

    player->position = position;
    strcpy(player->nickname, nickname);
    player->time = time;
    player->attempts = attempts;
    strcpy(player->date, date);

    memoryManager->addObject(player);

    return player;
}
