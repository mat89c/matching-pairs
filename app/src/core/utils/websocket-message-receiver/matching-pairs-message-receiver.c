#include "src/paths.h"
#include MATCHING_PAIRS_MESSAGE_RECEIVER
#include <stdio.h>

static bool isGameStarted(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), GAME_STARTED) == 0;
}

static void initCardIndices(cJSON *json, char **cardIndices) {
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON *receivedCardIndices = cJSON_GetObjectItem(data, "cardIndices");

    if (cJSON_IsArray(receivedCardIndices)) {
        for (int i = 0; i < cJSON_GetArraySize(receivedCardIndices); i++) {
            cJSON *item = cJSON_GetArrayItem(receivedCardIndices, i);
            if (cJSON_IsString(item)) {
                cardIndices[i] = item->valuestring;
            }
        }
    }
}

static bool isGameFinished(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), GAME_FINISHED) == 0;
}

static void gameFinished(cJSON *json, bool *isGameFinished, float *timer) {
    *isGameFinished = true;
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON *time = cJSON_GetObjectItem(data, "time");
    *timer = cJSON_GetNumberValue(time);
}

static bool isNicknameSubmitted(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), NICKNAME_SUBMITTED) == 0;
}

MatchingPairsMessageReceiver *createMatchingPairsMessageReceiver() {
    MatchingPairsMessageReceiver *matchingPairsMessageReceiver = malloc(sizeof(MatchingPairsMessageReceiver));
    if (NULL == matchingPairsMessageReceiver) {
        return NULL;
    }
    matchingPairsMessageReceiver->isGameStarted = isGameStarted;
    matchingPairsMessageReceiver->initCardIndices = initCardIndices;
    matchingPairsMessageReceiver->isGameFinished = isGameFinished;
    matchingPairsMessageReceiver->gameFinished = gameFinished;
    matchingPairsMessageReceiver->isNicknameSubmitted = isNicknameSubmitted;

    memoryManager->addObject(matchingPairsMessageReceiver);

    return matchingPairsMessageReceiver;
}
