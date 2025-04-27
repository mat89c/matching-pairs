#include "src/paths.h"
#include CARD
#include MATCHING_PAIRS

static void onFlip(Card *card, struct MatchingPairs *matchingPairs) {
    if (matchingPairs->firstFlippedCard == NULL) {
        matchingPairs->firstFlippedCard = card;
        return;
    }

    if (card == matchingPairs->firstFlippedCard) {
        return;
    }

    if (matchingPairs->secondFlippedCard == NULL) {
        matchingPairs->secondFlippedCard = card;
        matchingPairs->awaitingForMatch = true;
        matchingPairs->matchDelayTimer = 0.0f;
    }
}

static void update(Card *card, struct MatchingPairs *matchingPairs) {
    if (CheckCollisionPointRec(globalMouse->getMousePosition(), card->destination) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        card->isFlipped = true;
        card->onFlip(card, matchingPairs);
    }
}

static void draw(Card *card) {
    if (!card->isVisible) {
        return;
    }

    DrawTexture(card->frontTexture, card->destination.x, card->destination.y, WHITE);

    if (card->isFlipped) {
        DrawTexture(card->backTexture, card->destination.x, card->destination.y, WHITE);
        DrawText(TextFormat("Card %d", card->id), card->destination.x + 45, card->destination.y + 50, 20, BLACK);
    }
}

static void destroy(Card *card) {}

Card *createCard(CardTexture *cardTexture) {
    Card *card = malloc(sizeof(Card));
    if (NULL == card) {
        exit(EXIT_FAILURE);
    }

    card->destination = (Rectangle){0, 0, 0, 0};
    card->id = globalIdGenerator->generateId(globalIdGenerator);
    card->isFlipped = false;
    card->isMatched = false;
    card->frontTexture = cardTexture->frontTexture;
    card->backTexture = cardTexture->backTexture;
    card->destroy = destroy;
    card->draw = draw;
    card->update = update;
    card->onFlip = onFlip;
    card->isVisible = true;
    memoryManager->addObject(card);

    return card;
}

Card *cloneCard(Card *original) {
    Card *card = malloc(sizeof(Card));
    if (NULL == card) {
        exit(EXIT_FAILURE);
    }

    card->destination = (Rectangle){0, 0, 0, 0};
    card->id = original->id;
    card->isFlipped = false;
    card->isMatched = false;
    card->frontTexture = original->frontTexture;
    card->backTexture = original->backTexture;
    card->draw = draw;
    card->update = update;
    card->destroy = destroy;
    card->onFlip = onFlip;
    card->isVisible = original->isVisible;

    memoryManager->addObject(card);

    return card;
}
