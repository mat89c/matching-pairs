#ifndef CARD_TEXTURE_H
#define CARD_TEXTURE_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include "raylib.h"

typedef struct CardTexture CardTexture;

struct CardTexture {
    Texture2D frontTexture;
    Texture2D backTexture[9];
    void (*destroy)(CardTexture *cardTexture);
};

CardTexture *createCardTexture();

#endif
