#include "src/paths.h"
#include CARD_MESSAGE_RECEIVER
#include CARD

static bool isCardFlipped(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), CARD_FLIPPED) == 0;
}

static void flipCard(cJSON *json, Card **cards, CardTexture *cardTexture) {
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON *cardIndex = cJSON_GetObjectItem(data, "cardIndex");
    cJSON *cardTextureIndex = cJSON_GetObjectItem(data, "cardTextureIndex");

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        if (strcmp(cards[i]->index, cardIndex->valuestring) == 0) {
            cards[i]->isFlipped = true;
            cards[i]->backTexture = cardTexture->backTexture[cardTextureIndex->valueint];
        }
    }
}

static bool isCardsMatch(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), CARDS_MATCH) == 0;
}

static void matchCards(cJSON *json, Card **cards) {
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON *firstCardIndex = cJSON_GetObjectItem(data, "firstCardIndex");
    cJSON *secondCardIndex = cJSON_GetObjectItem(data, "secondCardIndex");

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        if (strcmp(cards[i]->index, firstCardIndex->valuestring) == 0) {
            cards[i]->isFlipped = true;
            cards[i]->backTexture = cards[i]->frontTexture;
            cards[i]->isVisible = false;
        }

        if (strcmp(cards[i]->index, secondCardIndex->valuestring) == 0) {
            cards[i]->isFlipped = true;
            cards[i]->backTexture = cards[i]->frontTexture;
            cards[i]->isVisible = false;
        }
    }
}

static bool isHideCards(cJSON *json) {
    return cJSON_IsString(cJSON_GetObjectItem(json, "event")) &&
           strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(json, "event")), HIDE_CARDS) == 0;
}

static void hideCards(cJSON *json, Card **cards, int *numberOfAttempts) {
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON *numberOfAttemptsJson = cJSON_GetObjectItem(data, "numberOfAttempts");
    *numberOfAttempts = cJSON_GetNumberValue(numberOfAttemptsJson);

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        cards[i]->backTexture = cards[i]->frontTexture;
        cards[i]->isFlipped = false;
    }
}

CardMessageReceiver *createCardMessageReceiver() {
    CardMessageReceiver *cardMessageReceiver = malloc(sizeof(CardMessageReceiver));
    if (NULL == cardMessageReceiver) {
        return NULL;
    }

    cardMessageReceiver->isCardFlipped = isCardFlipped;
    cardMessageReceiver->flipCard = flipCard;
    cardMessageReceiver->isCardsMatch = isCardsMatch;
    cardMessageReceiver->matchCards = matchCards;
    cardMessageReceiver->isHideCards = isHideCards;
    cardMessageReceiver->hideCards = hideCards;

    memoryManager->addObject(cardMessageReceiver);

    return cardMessageReceiver;
}
