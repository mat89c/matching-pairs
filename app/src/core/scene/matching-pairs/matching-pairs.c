#include "src/paths.h"
#include MATCHING_PAIRS
#include <stdio.h>

static void subscribeWebsocketMessage(void *self, cJSON *json) {
    MatchingPairs *matchingPairs = (MatchingPairs *)self;

    if (matchingPairs->matchingPairsMessageReceiver->isGameStarted(json)) {
        matchingPairs->matchingPairsMessageReceiver->initCardIndices(json, matchingPairs->cardIndices);

        matchingPairs->createCards(matchingPairs);
        matchingPairs->isDataLoaded = true;
    }

    if (matchingPairs->cardMessageReceiver->isCardFlipped(json)) {
        matchingPairs->cardMessageReceiver->flipCard(json, matchingPairs->cards, matchingPairs->cardTexture);
    }

    if (matchingPairs->cardMessageReceiver->isCardsMatch(json)) {
        matchingPairs->cardMessageReceiver->matchCards(json, matchingPairs->cards);
    }

    if (matchingPairs->cardMessageReceiver->isHideCards(json)) {
        matchingPairs->cardMessageReceiver->hideCards(json, matchingPairs->cards, &matchingPairs->numberOfAttempts);
    }

    if (matchingPairs->matchingPairsMessageReceiver->isGameFinished(json)) {
        matchingPairs->matchingPairsMessageReceiver->gameFinished(json, &matchingPairs->isGameFinished,
                                                                  &matchingPairs->timer);
    }

    if (matchingPairs->matchingPairsMessageReceiver->isNicknameSubmitted(json)) {
        matchingPairs->isNicknameSubmitted = true;
    }
}

static void createCards(MatchingPairs *matchingPairs) {
    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i] = createCard(matchingPairs->cardTexture, matchingPairs->cardIndices[i]);
        int row = i / NUMBER_OF_ROWS;
        int col = i % NUMBER_OF_COLUMNS;
        matchingPairs->cards[i]->destination = (Rectangle){
            130 + col * (CARD_WIDTH + SPACING), 100 + row * (CARD_HEIGHT + SPACING), CARD_WIDTH, CARD_HEIGHT};
    }
}

static void update(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    Vector2 mousePos = globalMouse->getMousePosition();
    if (CheckCollisionPointRec(mousePos, matchingPairs->returnBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        matchingPairs->clickSound->playSound(matchingPairs->clickSound);
        MainMenu *mainMenu = createMainMenu();
        sceneProvider->setScene((Scene *)mainMenu, sceneProvider);
    }

    if (matchingPairs->isNicknameSubmitted) {
        HallOfFame *hallOfFame = createHallOfFame();
        sceneProvider->setScene((Scene *)hallOfFame, sceneProvider);
    }

    if (matchingPairs->isGameFinished) {
        matchingPairs->submitBtnRect =
            (Rectangle){WINDOW_WIDTH / 2 - matchingPairs->submitBtnTexture.width / 2, 330,
                        matchingPairs->submitBtnTexture.width, matchingPairs->submitBtnTexture.height};

        if (CheckCollisionPointRec(mousePos, matchingPairs->submitBtnRect)) {
            matchingPairs->isSubmitBtnHovered = true;
        } else {
            matchingPairs->isSubmitBtnHovered = false;
        }

        if (CheckCollisionPointRec(mousePos, matchingPairs->submitBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            matchingPairs->clickSound->playSound(matchingPairs->clickSound);
            matchingPairs->matchingPairsMessageSender->submitNickname(matchingPairs->nickname);
        }

        return;
    }

    if (!matchingPairs->isDataLoaded) {
        return;
    }

    matchingPairs->timer += GetFrameTime();

    if (matchingPairs->isCardFlipped && matchingPairs->flippedCardIndex != NULL) {
        char *tempIndex = matchingPairs->flippedCardIndex;
        matchingPairs->cardMessageSender->flip(tempIndex);
        matchingPairs->isCardFlipped = false;
        matchingPairs->flippedCardIndex = NULL;
    }

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->update(matchingPairs->cards[i], matchingPairs);
    }
}

static void draw(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    ClearBackground(BLACK);
    DrawTexture(matchingPairs->backgroundTexture, 0, 0, WHITE);

    DrawText(TextFormat("Time: %.2f", matchingPairs->timer), 60, 30, 20, (Color){50, 50, 50, 255});
    DrawText(TextFormat("Attempts: %d", matchingPairs->numberOfAttempts), 60, 50, 20, (Color){50, 50, 50, 255});
    DrawTexture(matchingPairs->returnBtnTexture, matchingPairs->returnBtnRect.x, matchingPairs->returnBtnRect.y, WHITE);

    if (!matchingPairs->isDataLoaded) {
        return;
    }

    if (matchingPairs->isGameFinished) {
        DrawTexture(matchingPairs->youWonBackgroundTexture,
                    WINDOW_WIDTH / 2 - matchingPairs->youWonBackgroundTexture.width / 2, 100, WHITE);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        GuiSetStyle(TEXTBOX, BORDER_WIDTH, 1);
        GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0x323232FF);
        GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, 0x323232FF);
        GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, 0x323232FF);
        GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, 0xFF000000);
        GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, 0xFF000000);
        GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0xFF000000);
        GuiSetStyle(TEXTBOX, BASE_COLOR_DISABLED, 0xFF000000);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0xFF000000);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, 0xFF000000);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, 0x323232FF);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, 0xFF000000);

        if (GuiTextBox(matchingPairs->nicknameRect, matchingPairs->nickname, MAX_NICKNAME_LENGTH,
                       matchingPairs->isNicknameFocused)) {
            matchingPairs->isNicknameFocused = !matchingPairs->isNicknameFocused;
#if defined(PLATFORM_ANDROID)
            if (matchingPairs->isNicknameFocused) {
                androidBridge->showSoftKeyboard(matchingPairs->nickname, MAX_NICKNAME_LENGTH);
            } else {
                androidBridge->hideSoftKeyboard();
            }
#endif
        }

        if (matchingPairs->isSubmitBtnHovered) {
            DrawTexture(matchingPairs->submitBtnPressedTexture, matchingPairs->submitBtnRect.x,
                        matchingPairs->submitBtnRect.y, WHITE);
        } else {
            DrawTexture(matchingPairs->submitBtnTexture, matchingPairs->submitBtnRect.x, matchingPairs->submitBtnRect.y,
                        WHITE);
        }

        return;
    }

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        matchingPairs->cards[i]->draw(matchingPairs->cards[i]);
    }
}

static void destroy(Scene *scene) {
    MatchingPairs *matchingPairs = (MatchingPairs *)scene;

    if (matchingPairs->websocketSubscriber) {
        globalWebsocketManager->unregisterWebsocketSubscriber(globalWebsocketManager,
                                                              matchingPairs->websocketSubscriber);
    }

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        if (matchingPairs->cards[i] != NULL) {
            matchingPairs->cards[i]->destroy(matchingPairs->cards[i]);
        }
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
    matchingPairs->isCardFlipped = false;
    matchingPairs->flippedCardIndex = NULL;
    memset(matchingPairs->cards, 0, sizeof(Card *) * NUMBER_OF_CARDS);
    matchingPairs->numberOfAttempts = 0;
    matchingPairs->timer = 0.0f;
    matchingPairs->backgroundTexture = LoadTexture(ASSETS_PATH_PREFIX "images/game-bg.jpeg");
    matchingPairs->isGameFinished = false;
    matchingPairs->returnBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/return-btn.png");
    matchingPairs->returnBtnRect = (Rectangle){WINDOW_WIDTH - 120, 30, 100, 100};
    matchingPairs->createCards = createCards;
    memset(matchingPairs->cardIndices, 0, sizeof(matchingPairs->cardIndices));
    matchingPairs->isDataLoaded = false;
    matchingPairs->matchingPairsMessageSender = createMatchingPairsMessageSender();
    matchingPairs->cardMessageSender = createCardMessageSender();
    matchingPairs->matchingPairsMessageReceiver = createMatchingPairsMessageReceiver();
    matchingPairs->cardMessageReceiver = createCardMessageReceiver();
    matchingPairs->youWonBackgroundTexture = LoadTexture(ASSETS_PATH_PREFIX "images/you-won-header.png");
    matchingPairs->submitBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/submit-btn.png");
    matchingPairs->submitBtnPressedTexture = LoadTexture(ASSETS_PATH_PREFIX "images/submit-btn-pressed.png");
    matchingPairs->submitBtnRect = (Rectangle){0, 0, 0, 0};
    matchingPairs->isSubmitBtnHovered = false;
    matchingPairs->nickname[0] = '\0';
    matchingPairs->nicknameRect = (Rectangle){WINDOW_WIDTH / 2 - 101, 130, 202, 50};
    matchingPairs->isNicknameFocused = false;
    matchingPairs->isNicknameSubmitted = false;

    strcpy(matchingPairs->nickname, "Enter your name");

    matchingPairs->websocketSubscriber = createWebsocketSubscriber(matchingPairs, subscribeWebsocketMessage);
    globalWebsocketManager->registerWebsocketSubscriber(globalWebsocketManager, matchingPairs->websocketSubscriber);

    SoundManager *clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    matchingPairs->clickSound = clickSound;

    SoundManager *matchSound = createSoundManager(ASSETS_PATH_PREFIX "audio/match.ogg");
    matchingPairs->matchSound = matchSound;

    matchingPairs->matchingPairsMessageSender->startGame();

    memoryManager->addObject(matchingPairs);
    matchingPairs->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return matchingPairs;
}
