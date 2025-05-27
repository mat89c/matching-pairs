#include "src/paths.h"
#include HALL_OF_FAME_RECEIVER
#include <raylib.h>

bool isBestPlayersList(cJSON *cJSON) {
    TraceLog(LOG_INFO, "Checking if message is best players list");
    bool result = cJSON_IsString(cJSON_GetObjectItem(cJSON, "event")) &&
                  strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(cJSON, "event")), FETCH_BEST_PLAYERS_EVENT) == 0;
    TraceLog(LOG_INFO, "Message is%s best players list", result ? "" : " not");
    return result;
}

void bestPlayersList(cJSON *json, Player **players, int *playersCount, int *page, int *totalPlayers, int *totalPages) {
    TraceLog(LOG_INFO, "Processing best players list response");

    cJSON *data = cJSON_GetObjectItem(json, "data");
    if (!data) {
        TraceLog(LOG_ERROR, "No data field in response");
        return;
    }
    TraceLog(LOG_INFO, "Found data field in response");

    cJSON *collection = cJSON_GetObjectItem(data, "collection");
    if (!collection) {
        TraceLog(LOG_ERROR, "No collection field in data");
        return;
    }
    TraceLog(LOG_INFO, "Found collection field in data");

    cJSON *pageJson = cJSON_GetObjectItem(collection, "page");
    cJSON *totalJson = cJSON_GetObjectItem(collection, "total");
    if (!pageJson || !totalJson) {
        TraceLog(LOG_ERROR, "Missing page or total fields in collection");
        return;
    }
    TraceLog(LOG_INFO, "Found page and total fields in collection");

    *page = cJSON_GetNumberValue(pageJson);
    *totalPlayers = cJSON_GetNumberValue(totalJson);
    *totalPages = (*totalPlayers + 10 - 1) / 10;

    TraceLog(LOG_INFO, "Page: %d, Total Players: %d, Total Pages: %d", *page, *totalPlayers, *totalPages);

    cJSON *playersJson = cJSON_GetObjectItem(collection, "elements");
    if (!playersJson || !cJSON_IsArray(playersJson)) {
        TraceLog(LOG_ERROR, "No elements array in collection or invalid format");
        return;
    }
    TraceLog(LOG_INFO, "Found elements array in collection");

    int arraySize = cJSON_GetArraySize(playersJson);
    TraceLog(LOG_INFO, "Processing %d players", arraySize);

    for (int i = 0; i < arraySize; i++) {
        cJSON *playerJson = cJSON_GetArrayItem(playersJson, i);
        if (!playerJson) {
            TraceLog(LOG_ERROR, "Invalid player data at index %d", i);
            continue;
        }
        TraceLog(LOG_INFO, "Processing player at index %d", i);

        int position = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "position"));
        char *nickname = cJSON_GetStringValue(cJSON_GetObjectItem(playerJson, "nickname"));
        int time = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "time"));
        int attempts = cJSON_GetNumberValue(cJSON_GetObjectItem(playerJson, "attempts"));
        char *date = cJSON_GetStringValue(cJSON_GetObjectItem(playerJson, "date"));

        if (!nickname || !date) {
            TraceLog(LOG_ERROR, "Missing nickname or date for player at index %d", i);
            continue;
        }

        TraceLog(LOG_INFO, "Creating player object for %s", nickname);
        players[i] = createPlayer(position, nickname, time, attempts, date);
        TraceLog(LOG_INFO, "Processed player %d: %s", i + 1, nickname);
    }
    *playersCount = arraySize;
    TraceLog(LOG_INFO, "Finished processing players list");
}

HallOfFameReceiver *createHallOfFameReceiver() {
    TraceLog(LOG_INFO, "Creating Hall of Fame receiver");
    HallOfFameReceiver *hallOfFameReceiver = malloc(sizeof(HallOfFameReceiver));
    if (NULL == hallOfFameReceiver) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Hall of Fame receiver");
        exit(EXIT_FAILURE);
    }

    hallOfFameReceiver->isBestPlayersList = isBestPlayersList;
    hallOfFameReceiver->bestPlayersList = bestPlayersList;
    memoryManager->addObject(hallOfFameReceiver);

    TraceLog(LOG_INFO, "Hall of Fame receiver created successfully");
    return hallOfFameReceiver;
}
