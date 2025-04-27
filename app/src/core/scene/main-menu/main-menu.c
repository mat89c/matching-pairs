#include "src/paths.h"
#include MAIN_MENU

static void draw(Scene *scene) {
    MainMenu *mainMenu = (MainMenu *)scene;

    BeginTextureMode(sceneProvider->renderTarget);
    ClearBackground(RAYWHITE);
    DrawTexture(mainMenu->background, 0, 0, WHITE);
    DrawTexture(mainMenu->isStartGameBtnHovered ? mainMenu->startGameBtnPressed : mainMenu->startGameBtn,
                mainMenu->startGameBtnRect.x, mainMenu->startGameBtnRect.y, WHITE);
    DrawTexture(mainMenu->isQuitBtnHovered ? mainMenu->quitBtnPressed : mainMenu->quitBtn, mainMenu->quitBtnRect.x,
                mainMenu->quitBtnRect.y, WHITE);
    DrawTexture(mainMenu->isOptionsBtnHovered ? mainMenu->optionsBtnPressed : mainMenu->optionsBtn,
                mainMenu->optionsBtnRect.x, mainMenu->optionsBtnRect.y, WHITE);
    EndTextureMode();
}

static void update(Scene *scene) {
    MainMenu *mainMenu = (MainMenu *)scene;

    mainMenu->startGameBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->startGameBtn.width / 2.0f,
                                             WINDOW_HEIGHT / 2.0f - mainMenu->startGameBtn.height / 2.0f - 100.0f,
                                             mainMenu->startGameBtn.width, mainMenu->startGameBtn.height};

    mainMenu->quitBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->quitBtn.width / 2.0f,
                                        WINDOW_HEIGHT / 2.0f - mainMenu->quitBtn.height / 2.0f + 100.0f,
                                        mainMenu->quitBtn.width, mainMenu->quitBtn.height};

    mainMenu->optionsBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - mainMenu->optionsBtn.width / 2.0f,
                                           WINDOW_HEIGHT / 2.0f - mainMenu->optionsBtn.height / 2.0f,
                                           mainMenu->optionsBtn.width, mainMenu->optionsBtn.height};

    Vector2 mousePos = globalMouse->getMousePosition();

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

    if (CheckCollisionPointRec(mousePos, mainMenu->startGameBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);

        MatchingPairs *matchingPairs = createMatchingPairs();
        sceneProvider->setScene((Scene *)matchingPairs, sceneProvider);
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->quitBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);
        windowShouldClose = true;
    }

    if (CheckCollisionPointRec(mousePos, mainMenu->optionsBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mainMenu->clickSound->playSound(mainMenu->clickSound);
        Options *options = createOptions();
        sceneProvider->setScene((Scene *)options, sceneProvider);
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
    mainMenu->background = LoadTexture("app/assets/images/main-menu-bg.jpeg");
    mainMenu->startGameBtn = LoadTexture("app/assets/images/start-game-btn.png");
    mainMenu->startGameBtnPressed = LoadTexture("app/assets/images/start-game-btn-pressed.png");
    mainMenu->isStartGameBtnHovered = false;
    mainMenu->quitBtnRect = (Rectangle){0, 0, 0, 0};
    mainMenu->quitBtn = LoadTexture("app/assets/images/quit-btn.png");
    mainMenu->quitBtnPressed = LoadTexture("app/assets/images/quit-btn-pressed.png");
    mainMenu->isQuitBtnHovered = false;
    mainMenu->optionsBtnRect = (Rectangle){0, 0, 0, 0};
    mainMenu->optionsBtn = LoadTexture("app/assets/images/options-btn.png");
    mainMenu->optionsBtnPressed = LoadTexture("app/assets/images/options-btn-pressed.png");
    mainMenu->isOptionsBtnHovered = false;

    SoundManager *clickSound = createSoundManager("app/assets/audio/click.mp3");
    mainMenu->clickSound = clickSound;

    memoryManager->addObject(mainMenu);

    mainMenu->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return mainMenu;
}
