#include "src/paths.h"
#include MATCHING_PAIRS
#include <stdio.h>

static void update(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    if (matchingPairs->isFinished) {
        return;
    }

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
            matchingPairs->numberOfMatches++;
            matchingPairs->matchSound->playSound(matchingPairs->matchSound);
        }
    }

    if (matchingPairs->numberOfMatches == NUMBER_OF_PAIRS) {
        matchingPairs->isFinished = true;
    }

    Vector2 mousePos = globalMouse->getMousePosition();
    if (CheckCollisionPointRec(mousePos, matchingPairs->returnBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        matchingPairs->clickSound->playSound(matchingPairs->clickSound);
        MainMenu *mainMenu = createMainMenu();
        sceneProvider->setScene((Scene *)mainMenu, sceneProvider);
    }
}

static void draw(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    ClearBackground(RAYWHITE);
    DrawTexture(matchingPairs->backgroundTexture, 0, 0, WHITE);

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->draw(matchingPairs->cards[i]);
    }

    DrawText(TextFormat("Time: %.2f", matchingPairs->timer), 60, 30, 20, (Color){50, 50, 50, 255});
    DrawText(TextFormat("Attempts: %d", matchingPairs->numberOfAttempts), 60, 50, 20, (Color){50, 50, 50, 255});
    DrawTexture(matchingPairs->returnBtnTexture, matchingPairs->returnBtnRect.x, matchingPairs->returnBtnRect.y, WHITE);
}

static void destroy(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->destroy(matchingPairs->cards[i]);
    }

    UnloadTexture(matchingPairs->backgroundTexture);
    UnloadTexture(matchingPairs->returnBtnTexture);
    matchingPairs->cardTexture->destroy(matchingPairs->cardTexture);
    matchingPairs->clickSound->destroy(matchingPairs->clickSound);
    matchingPairs->matchSound->destroy(matchingPairs->matchSound);
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
    matchingPairs->cardTexture = createCardTexture();
    matchingPairs->firstFlippedCard = NULL;
    matchingPairs->secondFlippedCard = NULL;
    matchingPairs->numberOfMatches = 0;
    matchingPairs->numberOfAttempts = 0;
    matchingPairs->awaitingForMatch = false;
    matchingPairs->matchDelayTimer = 0.0f;
    matchingPairs->timer = 0.0f;
    matchingPairs->backgroundTexture = LoadTexture(ASSETS_PATH_PREFIX "images/game-bg.jpeg");
    matchingPairs->isFinished = false;
    matchingPairs->returnBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/return-btn.png");
    matchingPairs->returnBtnRect = (Rectangle){WINDOW_WIDTH - 120, 30, 100, 100};

    SoundManager *clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    matchingPairs->clickSound = clickSound;

    SoundManager *matchSound = createSoundManager(ASSETS_PATH_PREFIX "audio/match.ogg");
    matchingPairs->matchSound = matchSound;

    for (int i = 0; i < NUMBER_OF_PAIRS; i++) {
        matchingPairs->cards[i] = createCard(matchingPairs->cardTexture, i);
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
