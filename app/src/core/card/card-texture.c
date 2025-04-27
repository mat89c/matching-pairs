#include "src/paths.h"
#include CARD_TEXTURE

void destroy(CardTexture *cardTexture) {
    UnloadTexture(cardTexture->frontTexture);
    UnloadTexture(cardTexture->backTexture);
}

CardTexture *createCardTexture(const char *frontTexturePath, const char *backTexturePath) {
    CardTexture *cardTexture = malloc(sizeof(CardTexture));
    if (NULL == cardTexture) {
        exit(EXIT_FAILURE);
    }

    cardTexture->frontTexture = LoadTexture(frontTexturePath);
    cardTexture->backTexture = LoadTexture(backTexturePath);
    cardTexture->destroy = destroy;

    memoryManager->addObject(cardTexture);

    return cardTexture;
}
