#include "src/paths.h"
#include MAIN_MENU

static void draw(Scene *scene) {
    MainMenu *mainMenu = (MainMenu *)scene;

    ClearBackground(BLACK);
    DrawTexture(mainMenu->background, 0, 0, WHITE);
    DrawTexture(mainMenu->title, WINDOW_WIDTH / 2.0f - mainMenu->title.width / 2.0f, 50.0f, WHITE);

    DrawTexture(mainMenu->isHallOfFameBtnHovered ? mainMenu->hallOfFameBtnPressed : mainMenu->hallOfFameBtn,
                mainMenu->hallOfFameBtnRect.x, mainMenu->hallOfFameBtnRect.y, WHITE);
    DrawTexture(mainMenu->isStartGameBtnHovered ? mainMenu->startGameBtnPressed : mainMenu->startGameBtn,
                mainMenu->startGameBtnRect.x, mainMenu->startGameBtnRect.y, WHITE);
    DrawTexture(mainMenu->isOptionsBtnHovered ? mainMenu->optionsBtnPressed : mainMenu->optionsBtn,
                mainMenu->optionsBtnRect.x, mainMenu->optionsBtnRect.y, WHITE);

#if !defined(PLATFORM_WEB)
    DrawTexture(mainMenu->isQuitBtnHovered ? mainMenu->quitBtnPressed : mainMenu->quitBtn, mainMenu->quitBtnRect.x,
                mainMenu->quitBtnRect.y, WHITE);
#endif

    if (!globalWebsocketManager->isConnected(globalWebsocketManager)) {
        DrawText("Cannot connect to websocket server...", WINDOW_WIDTH / 2.0f - 200.0f, 273.0f, 20, RED);
    }
}

static void update(Scene *scene) {
    MainMenu *mainMenu = (MainMenu *)scene;

    Vector2 mousePos = globalMouse->getMousePosition();

    mainMenu->hallOfFameBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->hallOfFameBtn.width / 2.0f, 132.0f,
                                              mainMenu->hallOfFameBtn.width, mainMenu->hallOfFameBtn.height};

    mainMenu->startGameBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->startGameBtn.width / 2.0f,
                                             WINDOW_HEIGHT / 2.0f - mainMenu->startGameBtn.height / 2.0f,
                                             mainMenu->startGameBtn.width, mainMenu->startGameBtn.height};

    mainMenu->quitBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->quitBtn.width / 2.0f,
                                        WINDOW_HEIGHT / 2.0f - mainMenu->quitBtn.height / 2.0f + 200.0f,
                                        mainMenu->quitBtn.width, mainMenu->quitBtn.height};

    mainMenu->optionsBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->optionsBtn.width / 2.0f,
                                           WINDOW_HEIGHT / 2.0f - mainMenu->optionsBtn.height / 2.0f + 105.0f,
                                           mainMenu->optionsBtn.width, mainMenu->optionsBtn.height};

    if (CheckCollisionPointRec(mousePos, mainMenu->hallOfFameBtnRect)) {
        mainMenu->isHallOfFameBtnHovered = true;
    } else {
        mainMenu->isHallOfFameBtnHovered = false;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->startGameBtnRect)) {
        mainMenu->isStartGameBtnHovered = true;
    } else {
        mainMenu->isStartGameBtnHovered = false;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->optionsBtnRect)) {
        mainMenu->isOptionsBtnHovered = true;
    } else {
        mainMenu->isOptionsBtnHovered = false;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->quitBtnRect)) {
        mainMenu->isQuitBtnHovered = true;
    } else {
        mainMenu->isQuitBtnHovered = false;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->quitBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);
        windowShouldClose = true;
    }

    if (!globalWebsocketManager->isConnected(globalWebsocketManager)) {
        return;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->startGameBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);

        MatchingPairs *matchingPairs = createMatchingPairs();
        sceneProvider->setScene((Scene *)matchingPairs, sceneProvider);
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->optionsBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);
        Options *options = createOptions();
        sceneProvider->setScene((Scene *)options, sceneProvider);
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->hallOfFameBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);
        HallOfFame *hallOfFame = createHallOfFame();
        sceneProvider->setScene((Scene *)hallOfFame, sceneProvider);
    }
}

static void destroy(Scene *scene) {
    MainMenu *mainMenu = (MainMenu *)scene;
    UnloadTexture(mainMenu->background);
    UnloadTexture(mainMenu->startGameBtn);
    UnloadTexture(mainMenu->startGameBtnPressed);
    UnloadTexture(mainMenu->quitBtn);
    UnloadTexture(mainMenu->quitBtnPressed);
    UnloadTexture(mainMenu->optionsBtn);
    UnloadTexture(mainMenu->optionsBtnPressed);
    UnloadTexture(mainMenu->hallOfFameBtn);
    UnloadTexture(mainMenu->hallOfFameBtnPressed);
    mainMenu->clickSound->destroy(mainMenu->clickSound);
}

MainMenu *createMainMenu() {
    MainMenu *mainMenu = malloc(sizeof(MainMenu));

    if (NULL == mainMenu) {
        exit(EXIT_FAILURE);
    }

    mainMenu->base.name = "mainMenu";
    mainMenu->base.initialized = false;
    mainMenu->base.update = update;
    mainMenu->base.draw = draw;
    mainMenu->base.destroy = destroy;
    mainMenu->title = LoadTexture(ASSETS_PATH_PREFIX "images/main-menu-header.png");
    mainMenu->background = LoadTexture(ASSETS_PATH_PREFIX "images/game-bg.jpeg");
    mainMenu->startGameBtn = LoadTexture(ASSETS_PATH_PREFIX "images/start-game-btn.png");
    mainMenu->startGameBtnPressed = LoadTexture(ASSETS_PATH_PREFIX "images/start-game-btn-pressed.png");
    mainMenu->startGameBtnRect = (Rectangle){0, 0, 0, 0};
    mainMenu->isStartGameBtnHovered = false;
    mainMenu->quitBtnRect = (Rectangle){0, 0, 0, 0};
    mainMenu->quitBtn = LoadTexture(ASSETS_PATH_PREFIX "images/quit-btn.png");
    mainMenu->quitBtnPressed = LoadTexture(ASSETS_PATH_PREFIX "images/quit-btn-pressed.png");
    mainMenu->isQuitBtnHovered = false;
    mainMenu->optionsBtnRect = (Rectangle){0, 0, 0, 0};
    mainMenu->optionsBtn = LoadTexture(ASSETS_PATH_PREFIX "images/options-btn.png");
    mainMenu->optionsBtnPressed = LoadTexture(ASSETS_PATH_PREFIX "images/options-btn-pressed.png");
    mainMenu->isOptionsBtnHovered = false;
    mainMenu->hallOfFameBtn = LoadTexture(ASSETS_PATH_PREFIX "images/hall-of-fame-btn.png");
    mainMenu->hallOfFameBtnPressed = LoadTexture(ASSETS_PATH_PREFIX "images/hall-of-fame-btn-pressed.png");
    mainMenu->isHallOfFameBtnHovered = false;
    mainMenu->hallOfFameBtnRect = (Rectangle){0, 0, 0, 0};

    SoundManager *clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");
    mainMenu->clickSound = clickSound;

    memoryManager->addObject(mainMenu);

    mainMenu->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return mainMenu;
}
