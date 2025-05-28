#include "src/paths.h"
#include HALL_OF_FAME_RECEIVER
#include <raylib.h>

bool isBestPlayersList(cJSON *cJSON) {
    bool result = cJSON_IsString(cJSON_GetObjectItem(cJSON, "event")) &&
                  strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(cJSON, "event")), FETCH_BEST_PLAYERS_EVENT) == 0;
    return result;
}

void bestPlayersList(cJSON *json, Player **players, int *playersCount, int *page, int *totalPlayers, int *totalPages) {

    cJSON *data = cJSON_GetObjectItem(json, "data");
    if (!data) {
        return;
    }

    cJSON *collection = cJSON_GetObjectItem(data, "collection");
    if (!collection) {
        return;
    }

    cJSON *pageJson = cJSON_GetObjectItem(collection, "page");
    cJSON *totalJson = cJSON_GetObjectItem(collection, "total");
    if (!pageJson || !totalJson) {
        return;
    }

    *page = cJSON_GetNumberValue(pageJson);
    *totalPlayers = cJSON_GetNumberValue(totalJson);
    *totalPages = (*totalPlayers + 10 - 1) / 10;

    cJSON *playersJson = cJSON_GetObjectItem(collection, "elements");
    if (!playersJson || !cJSON_IsArray(playersJson)) {
        return;
    }

    int arraySize = cJSON_GetArraySize(playersJson);

    for (int i = 0; i < arraySize; i++) {
        cJSON *playerJson = cJSON_GetArrayItem(playersJson, i);
        if (!playerJson) {
            continue;
        }

        int position = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "position"));
        char *nickname = cJSON_GetStringValue(cJSON_GetObjectItem(playerJson, "nickname"));
        int time = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "time"));
        int attempts = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "attempts"));
        char *date = cJSON_GetStringValue(cJSON_GetObjectItem(playerJson, "date"));

        if (!nickname || !date) {
            continue;
        }

        players[i] = createPlayer(position, nickname, time, attempts, date);
    }
    *playersCount = arraySize;
}

HallOfFameReceiver *createHallOfFameReceiver() {
    HallOfFameReceiver *hallOfFameReceiver = malloc(sizeof(HallOfFameReceiver));
    if (NULL == hallOfFameReceiver) {
        exit(EXIT_FAILURE);
    }

    hallOfFameReceiver->isBestPlayersList = isBestPlayersList;
    hallOfFameReceiver->bestPlayersList = bestPlayersList;
    memoryManager->addObject(hallOfFameReceiver);

    return hallOfFameReceiver;
}
