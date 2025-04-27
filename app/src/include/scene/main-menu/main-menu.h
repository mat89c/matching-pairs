#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "src/paths.h"
#include SCENE
#include MEMORY_MANAGER
#include SOUND_MANAGER
#include WINDOW_MANAGER
#include SCENE_PROVIDER
#include OPTIONS
#include MATCHING_PAIRS
#include GLOBAL_MOUSE
#include <math.h>
#include <stdbool.h>

typedef struct MainMenu MainMenu;

struct MainMenu {
    Scene base;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    Texture2D background;
    Rectangle startGameBtnRect;
    Texture2D startGameBtn;
    Texture2D startGameBtnPressed;
    bool isStartGameBtnHovered;
    Rectangle quitBtnRect;
    Texture2D quitBtn;
    Texture2D quitBtnPressed;
    bool isQuitBtnHovered;
    Rectangle optionsBtnRect;
    Texture2D optionsBtn;
    Texture2D optionsBtnPressed;
    bool isOptionsBtnHovered;
    SoundManager *clickSound;
};

MainMenu *createMainMenu();

#endif
