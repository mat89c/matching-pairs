#include "src/paths.h"
#include CARD_TEXTURE

static void destroy(CardTexture *cardTexture) {
    UnloadTexture(cardTexture->frontTexture);
    UnloadTexture(cardTexture->backTexture[0]);
    UnloadTexture(cardTexture->backTexture[1]);
    UnloadTexture(cardTexture->backTexture[2]);
    UnloadTexture(cardTexture->backTexture[3]);
    UnloadTexture(cardTexture->backTexture[4]);
    UnloadTexture(cardTexture->backTexture[5]);
    UnloadTexture(cardTexture->backTexture[6]);
    UnloadTexture(cardTexture->backTexture[7]);
    UnloadTexture(cardTexture->backTexture[8]);
}

CardTexture *createCardTexture() {
    CardTexture *cardTexture = malloc(sizeof(CardTexture));
    if (NULL == cardTexture) {
        exit(EXIT_FAILURE);
    }

    cardTexture->frontTexture = LoadTexture(ASSETS_PATH_PREFIX "images/card-front.png");
    cardTexture->backTexture[0] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-1.png");
    cardTexture->backTexture[1] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-2.png");
    cardTexture->backTexture[2] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-3.png");
    cardTexture->backTexture[3] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-4.png");
    cardTexture->backTexture[4] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-5.png");
    cardTexture->backTexture[5] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-6.png");
    cardTexture->backTexture[6] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-7.png");
    cardTexture->backTexture[7] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-8.png");
    cardTexture->backTexture[8] = LoadTexture(ASSETS_PATH_PREFIX "images/card-back-9.png");

    cardTexture->destroy = destroy;

    memoryManager->addObject(cardTexture);

    return cardTexture;
}
