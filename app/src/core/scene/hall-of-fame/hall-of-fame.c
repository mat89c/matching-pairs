#include "src/paths.h"
#include HALL_OF_FAME

void subscribeWebsocketMessage(void *self, cJSON *json) {
    HallOfFame *hallOfFame = (HallOfFame *)self;

    if (hallOfFame->hallOfFameMessageReceiver->isBestPlayersList(json)) {
        hallOfFame->hallOfFameMessageReceiver->bestPlayersList(json, hallOfFame->players, &hallOfFame->playersCount,
                                                               &hallOfFame->currentPage, &hallOfFame->totalPlayers,
                                                               &hallOfFame->totalPages);
    }
}

void update(Scene *scene) {
    HallOfFame *hallOfFame = (HallOfFame *)scene;

    Vector2 mousePos = globalMouse->getMousePosition();
    if (CheckCollisionPointRec(mousePos, hallOfFame->returnBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        hallOfFame->clickSound->playSound(hallOfFame->clickSound);
        MainMenu *mainMenu = createMainMenu();
        sceneProvider->setScene((Scene *)mainMenu, sceneProvider);
    }

    if (CheckCollisionPointRec(mousePos, hallOfFame->prevBtnRect) && hallOfFame->currentPage > 1 &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        hallOfFame->clickSound->playSound(hallOfFame->clickSound);
        hallOfFame->currentPage--;
        hallOfFame->hallOfFameMessageSender->fetchBestPlayers(hallOfFame->currentPage);
    }

    if (CheckCollisionPointRec(mousePos, hallOfFame->nextBtnRect) && hallOfFame->currentPage < hallOfFame->totalPages &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        hallOfFame->clickSound->playSound(hallOfFame->clickSound);
        hallOfFame->currentPage++;
        hallOfFame->hallOfFameMessageSender->fetchBestPlayers(hallOfFame->currentPage);
    }
}

void draw(Scene *scene) {
    HallOfFame *hallOfFame = (HallOfFame *)scene;
    ClearBackground(BLACK);
    DrawTexture(hallOfFame->background, 0, 0, WHITE);
    DrawTexture(hallOfFame->returnBtnTexture, hallOfFame->returnBtnRect.x, hallOfFame->returnBtnRect.y, WHITE);
    DrawTexture(hallOfFame->headerTexture, WINDOW_WIDTH / 2 - hallOfFame->headerTexture.width / 2, 30, WHITE);

    DrawText("Nickname", 150, 120, 24, (Color){23, 23, 23, 255});
    DrawText("Attempts", 450, 120, 24, (Color){23, 23, 23, 255});
    DrawText("Time", 650, 120, 24, (Color){23, 23, 23, 255});
    DrawText("Date", 950, 120, 24, (Color){23, 23, 23, 255});

    for (int i = 0; i < hallOfFame->playersCount; i++) {
        char positionText[16];
        char attempts[16];
        char time[16];
        sprintf(positionText, "%d.", hallOfFame->players[i]->position);
        sprintf(attempts, "%d", hallOfFame->players[i]->attempts);
        sprintf(time, "%d sec", hallOfFame->players[i]->time);

        DrawText(positionText, 100, 175 + i * 40, 20, (Color){23, 23, 23, 255});
        DrawText(hallOfFame->players[i]->nickname, 150, 175 + i * 40, 20, (Color){23, 23, 23, 255});
        DrawText(attempts, 450, 175 + i * 40, 20, (Color){23, 23, 23, 255});
        DrawText(time, 650, 175 + i * 40, 20, (Color){23, 23, 23, 255});
        DrawText(hallOfFame->players[i]->date, 950, 175 + i * 40, 20, (Color){23, 23, 23, 255});
    }

    char currentPageText[16];
    sprintf(currentPageText, "%d", hallOfFame->currentPage);
    if (hallOfFame->currentPage > 1) {
        DrawTexture(hallOfFame->prevBtnTexture, hallOfFame->prevBtnRect.x, hallOfFame->prevBtnRect.y, WHITE);
    }

    DrawText(currentPageText, WINDOW_WIDTH / 2 + 7, WINDOW_HEIGHT - 100, 20, (Color){23, 23, 23, 255});

    if (hallOfFame->currentPage < hallOfFame->totalPages) {
        DrawTexture(hallOfFame->nextBtnTexture, hallOfFame->nextBtnRect.x, hallOfFame->nextBtnRect.y, WHITE);
    }
}

void destroy(Scene *scene) {
    HallOfFame *hallOfFame = (HallOfFame *)scene;
    UnloadTexture(hallOfFame->background);
    UnloadTexture(hallOfFame->returnBtnTexture);
    UnloadTexture(hallOfFame->headerTexture);
    UnloadTexture(hallOfFame->nextBtnTexture);
    UnloadTexture(hallOfFame->prevBtnTexture);
    hallOfFame->clickSound->destroy(hallOfFame->clickSound);

    if (hallOfFame->websocketSubscriber) {
        globalWebsocketManager->unregisterWebsocketSubscriber(globalWebsocketManager, hallOfFame->websocketSubscriber);
    }

    free(hallOfFame->players);
}

HallOfFame *createHallOfFame() {
    HallOfFame *hallOfFame = malloc(sizeof(HallOfFame));
    if (NULL == hallOfFame) {
        exit(EXIT_FAILURE);
    }

    hallOfFame->base.name = "hall-of-fame";
    hallOfFame->base.initialized = false;
    hallOfFame->base.update = update;
    hallOfFame->base.draw = draw;
    hallOfFame->base.destroy = destroy;

    hallOfFame->background = LoadTexture(ASSETS_PATH_PREFIX "images/game-bg.jpeg");
    hallOfFame->returnBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/return-btn.png");
    hallOfFame->returnBtnRect = (Rectangle){WINDOW_WIDTH - 120, 30, 100, 100};
    hallOfFame->clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    hallOfFame->hallOfFameMessageReceiver = createHallOfFameReceiver();
    hallOfFame->hallOfFameMessageSender = createHallOfFameSender();
    hallOfFame->currentPage = 1;
    hallOfFame->totalPages = 1;
    hallOfFame->playersCount = 0;
    hallOfFame->players = malloc(sizeof(Player *) * MAX_PLAYERS);
    hallOfFame->headerTexture = LoadTexture(ASSETS_PATH_PREFIX "images/hall-of-fame-dark.png");
    hallOfFame->nextBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/right-btn.png");
    hallOfFame->prevBtnTexture = LoadTexture(ASSETS_PATH_PREFIX "images/left-btn.png");
    hallOfFame->nextBtnRect = (Rectangle){WINDOW_WIDTH / 2 + 35, WINDOW_HEIGHT - 100, 100, 100};
    hallOfFame->prevBtnRect = (Rectangle){WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT - 100, 100, 100};

    hallOfFame->websocketSubscriber = createWebsocketSubscriber(hallOfFame, subscribeWebsocketMessage);
    globalWebsocketManager->registerWebsocketSubscriber(globalWebsocketManager, hallOfFame->websocketSubscriber);

    hallOfFame->hallOfFameMessageSender->fetchBestPlayers(hallOfFame->currentPage);

    memoryManager->addObject(hallOfFame);
    hallOfFame->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return hallOfFame;
}
