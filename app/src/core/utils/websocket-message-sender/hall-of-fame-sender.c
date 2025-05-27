#include "src/paths.h"
#include HALL_OF_FAME_SENDER
#include <raylib.h>

void fetchBestPlayers(int page) {
    TraceLog(LOG_INFO, "Creating fetch best players request for page %d", page);

    cJSON *json = cJSON_CreateObject();
    if (!json) {
        TraceLog(LOG_ERROR, "Failed to create JSON object");
        return;
    }

    cJSON_AddStringToObject(json, "event", FETCH_BEST_PLAYERS_EVENT);

    cJSON *data = cJSON_CreateObject();
    if (!data) {
        TraceLog(LOG_ERROR, "Failed to create data JSON object");
        cJSON_Delete(json);
        return;
    }

    cJSON_AddNumberToObject(data, "page", page);
    cJSON_AddItemToObject(json, "data", data);

    TraceLog(LOG_INFO, "Sending websocket message");
    globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, json);
    cJSON_Delete(json);
    TraceLog(LOG_INFO, "Websocket message sent");
}

HallOfFameSender *createHallOfFameSender() {
    TraceLog(LOG_INFO, "Creating Hall of Fame sender");
    HallOfFameSender *hallOfFameSender = malloc(sizeof(HallOfFameSender));
    if (NULL == hallOfFameSender) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Hall of Fame sender");
        exit(EXIT_FAILURE);
    }

    hallOfFameSender->fetchBestPlayers = fetchBestPlayers;
    memoryManager->addObject(hallOfFameSender);

    TraceLog(LOG_INFO, "Hall of Fame sender created successfully");
    return hallOfFameSender;
}
