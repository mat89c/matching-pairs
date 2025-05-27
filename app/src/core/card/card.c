#include "src/paths.h"
#include CARD

// static void subscribeWebsocketMessage(void *self, cJSON *json) { Card *card = (Card *)self; }

static void onFlip(Card *card, struct MatchingPairs *matchingPairs) {
    // matchingPairs->isCardFlipped = true;
    // matchingPairs->flippedCardIndex = card->index;
    // if (matchingPairs->firstFlippedCard == NULL) {
    //     matchingPairs->firstFlippedCard = card;
    //     return;
    // }

    // if (card == matchingPairs->firstFlippedCard) {
    //     return;
    // }

    // if (matchingPairs->secondFlippedCard == NULL) {
    //     matchingPairs->secondFlippedCard = card;
    //     matchingPairs->awaitingForMatch = true;
    //     matchingPairs->matchDelayTimer = 0.0f;
    // }
    // globalWebsocketManager->sendWebsocketMessage(globalWebsocketManager, "card:flipped", card->id);
}

static void update(Card *card, struct MatchingPairs *matchingPairs) {
    if (CheckCollisionPointRec(globalMouse->getMousePosition(), card->destination) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        matchingPairs->isCardFlipped = true;
        matchingPairs->flippedCardIndex = card->index;
        card->clickSound->playSound(card->clickSound);
    }
}

static void draw(Card *card) {
    if (!card->isVisible) {
        return;
    }

    DrawTexture(card->frontTexture, card->destination.x, card->destination.y, WHITE);

    if (card->isFlipped) {
        DrawTexture(card->backTexture, card->destination.x, card->destination.y, WHITE);
    }
}

static void destroy(Card *card) {
    UnloadTexture(card->frontTexture);
    UnloadTexture(card->backTexture);
    card->clickSound->destroy(card->clickSound);
    free(card->index);
}

Card *createCard(CardTexture *cardTexture, char *index) {
    Card *card = malloc(sizeof(Card));
    if (NULL == card) {
        exit(EXIT_FAILURE);
    }

    card->destination = (Rectangle){0, 0, 0, 0};
    card->index = index != NULL ? strdup(index) : NULL;
    card->isFlipped = false;
    card->frontTexture = cardTexture->frontTexture;
    card->backTexture = cardTexture->frontTexture;
    card->destroy = destroy;
    card->draw = draw;
    card->update = update;
    card->onFlip = onFlip;
    card->isVisible = true;

    SoundManager *clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    card->clickSound = clickSound;

    memoryManager->addObject(card);

    return card;
}
