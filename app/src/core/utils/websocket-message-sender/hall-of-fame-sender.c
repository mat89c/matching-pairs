#include "src/paths.h"
#include HALL_OF_FAME_SENDER
#include <raylib.h>

void fetchBestPlayers(int page) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        return;
    }

    cJSON_AddStringToObject(json, "event", FETCH_BEST_PLAYERS_EVENT);

    cJSON *data = cJSON_CreateObject();
    if (!data) {
        cJSON_Delete(json);
        return;
    }

    cJSON_AddNumberToObject(data, "page", page);
    cJSON_AddItemToObject(json, "data", data);

    globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, json);
    cJSON_Delete(json);
}

HallOfFameSender *createHallOfFameSender() {
    HallOfFameSender *hallOfFameSender = malloc(sizeof(HallOfFameSender));
    if (NULL == hallOfFameSender) {
        exit(EXIT_FAILURE);
    }

    hallOfFameSender->fetchBestPlayers = fetchBestPlayers;
    memoryManager->addObject(hallOfFameSender);

    return hallOfFameSender;
}
