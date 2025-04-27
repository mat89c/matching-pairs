#ifndef OPTIONS_H
#define OPTIONS_H

#include "src/paths.h"
#include SCENE
#include MEMORY_MANAGER
#include SOUND_MANAGER
#include SCENE_PROVIDER
#include MAIN_MENU
#include GLOBAL_GAME_OPTIONS
#include GLOBAL_MOUSE
#include RAYGUI
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Options Options;

struct Options {
    Scene base;
    bool initialized;
    void (*update)(Scene *scene);
    void (*draw)(Scene *scene);
    void (*destroy)(Scene *scene);
    Texture2D background;
    Rectangle backBtnRect;
    Texture2D backBtn;
    Texture2D backBtnPressed;
    bool isBackBtnHovered;
    SoundManager *clickSound;
};

Options *createOptions(void);

#endif
