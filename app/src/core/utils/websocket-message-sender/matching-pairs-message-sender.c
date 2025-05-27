#include "src/paths.h"
#include MATCHING_PAIRS_MESSAGE_SENDER

static void startGame() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "event", GAME_START);
    globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, json);
    cJSON_Delete(json);
}

static void submitNickname(char *nickname) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "event", NICKNAME_SUBMIT);
    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "nickname", nickname);
    cJSON_AddItemToObject(json, "data", data);

    globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, json);
    cJSON_Delete(json);
}
MatchingPairsMessageSender *createMatchingPairsMessageSender() {
    MatchingPairsMessageSender *matchingPairsMessageSender = malloc(sizeof(MatchingPairsMessageSender));
    if (NULL == matchingPairsMessageSender) {
        exit(EXIT_FAILURE);
    }

    matchingPairsMessageSender->startGame = startGame;
    matchingPairsMessageSender->submitNickname = submitNickname;

    memoryManager->addObject(matchingPairsMessageSender);

    return matchingPairsMessageSender;
}
