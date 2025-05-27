
#include "src/paths.h"
#include CARD_MESSAGE_SENDER

static void flip(char *cardIndex) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "event", CARD_FLIP);

    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "cardIndex", cardIndex);
    cJSON_AddItemToObject(json, "data", data);
    globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, json);
    cJSON_Delete(json);
}

CardMessageSender *createCardMessageSender() {
    CardMessageSender *cardMessageSender = malloc(sizeof(CardMessageSender));
    if (NULL == cardMessageSender) {
        exit(EXIT_FAILURE);
    }

    cardMessageSender->flip = flip;

    memoryManager->addObject(cardMessageSender);

    return cardMessageSender;
}
