#include "src/paths.h"
#include OPTIONS

static void update(Scene *scene) {
    Options *options = (Options *)scene;

    options->backBtnRect = (Rectangle){WINDOW_WIDTH / 2.0f - options->backBtn.width / 2.0f,
                                       WINDOW_HEIGHT / 2.0f - options->backBtn.height / 2.0f + 100.0f,
                                       options->backBtn.width, options->backBtn.height};

    Vector2 mousePos = globalMouse->getMousePosition();
    if (CheckCollisionPointRec(mousePos, options->backBtnRect)) {
        options->isBackBtnHovered = true;
    } else {
        options->isBackBtnHovered = false;
    }

    if (CheckCollisionPointRec(mousePos, options->backBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        options->clickSound->playSound(options->clickSound);
        MainMenu *mainMenu = createMainMenu();
        sceneProvider->setScene((Scene *)mainMenu, sceneProvider);
    }
}

static void draw(Scene *scene) {
    Options *options = (Options *)scene;

    char musicVolStr[16];
    char sfxVolStr[16];
    sprintf(musicVolStr, "%.0f", globalGameOptions->musicVolume);
    sprintf(sfxVolStr, "%.0f", globalGameOptions->sfxVolume);

    ClearBackground(RAYWHITE);
    DrawTexture(options->background, 0, 0, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, 0xFFC800ff);
    GuiSlider((Rectangle){WINDOW_WIDTH / 2.0f - 200, 200, 400, 40}, "Music volume: ", musicVolStr,
              &globalGameOptions->musicVolume, 0, 100);
    GuiSlider((Rectangle){WINDOW_WIDTH / 2.0f - 200, 250, 400, 40}, "Effects volume: ", sfxVolStr,
              &globalGameOptions->sfxVolume, 0, 100);

    DrawTexture(options->isBackBtnHovered ? options->backBtnPressed : options->backBtn, options->backBtnRect.x,
                options->backBtnRect.y, WHITE);
}

static void destroy(Scene *scene) {
    Options *options = (Options *)scene;
    UnloadTexture(options->background);
    UnloadTexture(options->backBtn);
    UnloadTexture(options->backBtnPressed);
    options->clickSound->destroy(options->clickSound);
}

Options *createOptions(void) {
    Options *options = malloc(sizeof(Options));

    if (NULL == options) {
        exit(EXIT_FAILURE);
    }

    options->base.name = "options";
    options->base.initialized = false;
    options->base.update = update;
    options->base.draw = draw;
    options->base.destroy = destroy;
    options->background = LoadTexture(ASSETS_PATH_PREFIX "images/game-bg.jpeg");
    options->backBtn = LoadTexture(ASSETS_PATH_PREFIX "images/back-btn.png");
    options->backBtnPressed = LoadTexture(ASSETS_PATH_PREFIX "images/back-btn-pressed.png");
    options->isBackBtnHovered = false;
    options->clickSound = createSoundManager(ASSETS_PATH_PREFIX "audio/click.mp3");

    memoryManager->addObject(options);

    options->base.memoryManagerIndex = memoryManager->nextIndex(memoryManager);

    return options;
}
