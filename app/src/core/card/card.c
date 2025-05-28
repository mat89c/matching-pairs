#include "src/paths.h"
#include CARD

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
    card->isVisible = true;

    SoundManager *clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    card->clickSound = clickSound;

    memoryManager->addObject(card);

    return card;
}
