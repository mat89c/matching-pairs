#include "src/paths.h"
#include MATCHING_PAIRS
#include <stdio.h>

static void update(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;
    matchingPairs->timer += GetFrameTime();

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->update(matchingPairs->cards[i], matchingPairs);

        if (!matchingPairs->awaitingForMatch) {
            continue;
        }

        matchingPairs->matchDelayTimer += GetFrameTime();

        if (matchingPairs->matchDelayTimer < MATCH_DELAY_TIME) {
            continue;
        }

        matchingPairs->awaitingForMatch = false;
        matchingPairs->matchDelayTimer = 0.0f;

        matchingPairs->numberOfAttempts++;

        if (matchingPairs->firstFlippedCard->id != matchingPairs->secondFlippedCard->id) {
            matchingPairs->firstFlippedCard->isFlipped = false;
            matchingPairs->secondFlippedCard->isFlipped = false;
            matchingPairs->firstFlippedCard = NULL;
            matchingPairs->secondFlippedCard = NULL;
            continue;
        }

        if (matchingPairs->firstFlippedCard->id == matchingPairs->secondFlippedCard->id) {
            matchingPairs->firstFlippedCard->isVisible = false;
            matchingPairs->secondFlippedCard->isVisible = false;
            matchingPairs->firstFlippedCard = NULL;
            matchingPairs->secondFlippedCard = NULL;
        }
    }
}

static void draw(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    BeginTextureMode(sceneProvider->renderTarget);
    ClearBackground(RAYWHITE);
    DrawTexture(matchingPairs->backgroundTexture, 0, 0, WHITE);

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->draw(matchingPairs->cards[i]);
    }

    DrawText(TextFormat("Time: %.2f", matchingPairs->timer), 10, 10, 20, BLACK);
    DrawText(TextFormat("Attempts: %d", matchingPairs->numberOfAttempts), 10, 30, 20, BLACK);
    EndTextureMode();
}

static void destroy(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->destroy(matchingPairs->cards[i]);
    }

    UnloadTexture(matchingPairs->backgroundTexture);
    matchingPairs->cardTexture->destroy(matchingPairs->cardTexture);
}

MatchingPairs *createMatchingPairs() {
    MatchingPairs *matchingPairs = malloc(sizeof(MatchingPairs));
    if (NULL == matchingPairs) {
        exit(EXIT_FAILURE);
    }

    matchingPairs->base.initialized = false;
    matchingPairs->base.update = update;
    matchingPairs->base.draw = draw;
    matchingPairs->base.destroy = destroy;
    matchingPairs->cardTexture =
        createCardTexture("app/assets/images/card-front.png", "app/assets/images/card-back.png");
    matchingPairs->firstFlippedCard = NULL;
    matchingPairs->secondFlippedCard = NULL;
    matchingPairs->numberOfAttempts = 0;
    matchingPairs->awaitingForMatch = false;
    matchingPairs->matchDelayTimer = 0.0f;
    matchingPairs->timer = 0.0f;
    matchingPairs->backgroundTexture = LoadTexture("app/assets/images/game-bg.jpeg");

    for (int i = 0; i < NUMBER_OF_PAIRS; i++) {
        matchingPairs->cards[i] = createCard(matchingPairs->cardTexture);
    }

    for (int i = 0; i < NUMBER_OF_PAIRS; i++) {
        matchingPairs->cards[i + NUMBER_OF_PAIRS] = cloneCard(matchingPairs->cards[i]);
    }

    for (int i = NUMBER_OF_CARDS - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);

        Card *temp = matchingPairs->cards[i];
        matchingPairs->cards[i] = matchingPairs->cards[j];
        matchingPairs->cards[j] = temp;
    }

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        int row = i / NUMBER_OF_ROWS;
        int col = i % NUMBER_OF_COLUMNS;
        matchingPairs->cards[i]->destination = (Rectangle){
            130 + col * (CARD_WIDTH + SPACING), 100 + row * (CARD_HEIGHT + SPACING), CARD_WIDTH, CARD_HEIGHT};
    }

    memoryManager->addObject(matchingPairs);

    matchingPairs->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return matchingPairs;
}
