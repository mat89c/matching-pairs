#ifndef PATHS_H
#define PATHS_H

#if defined(PLATFORM_ANDROID)
#define ASSETS_PATH_PREFIX ""
#elif defined(PLATFORM_WEB)
#define ASSETS_PATH_PREFIX "/app/assets/"
#else
#define ASSETS_PATH_PREFIX "assets/"
#endif

#define WINDOW_MANAGER "src/include/utils/window-manager/window-manager.h"
#define MEMORY_MANAGER "src/include/utils/memory-manager/memory-manager.h"
#define SCENE_MANAGER "src/include/utils/scene-manager/scene-manager.h"
#define SCENE "src/include/scene/scene.h"
#define SCENE_PROVIDER "src/include/scene/scene-provider.h"
#define MAIN_MENU "src/include/scene/main-menu/main-menu.h"
#define MUSIC_MANAGER "src/include/utils/music-manager/music-manager.h"
#define SOUND_MANAGER "src/include/utils/sound-manager/sound-manager.h"
#define OPTIONS "src/include/scene/options/options.h"
#define RAYGUI "src/../libs/raygui.h"
#define GLOBAL_GAME_OPTIONS "src/include/utils/global-game-options/global-game-options.h"
#define MATCHING_PAIRS "src/include/scene/matching-pairs/matching-pairs.h"
#define CARD "src/include/card/card.h"
#define CARD_TEXTURE "src/include/card/card-texture.h"
#define GLOBAL_ID_GENERATOR "src/include/utils/global-id-generator/global-id-generator.h"
#define GLOBAL_MOUSE "src/include/utils/global-mouse/global-mouse.h"
#define GLOBAL_WEBSOCKET_MANAGER "src/include/utils/websocket-manager/global-websocket-manager.h"
#define WEBSOCKET_SUBSCRIBER "src/include/utils/websocket-manager/websocket-subscriber.h"
#define MATCHING_PAIRS_MESSAGE_SENDER "src/include/utils/websocket-message-sender/matching-pairs-message-sender.h"
#define CARD_MESSAGE_SENDER "src/include/utils/websocket-message-sender/card-message-sender.h"
#define MATCHING_PAIRS_MESSAGE_RECEIVER "src/include/utils/websocket-message-receiver/matching-pairs-message-receiver.h"
#define CARD_MESSAGE_RECEIVER "src/include/utils/websocket-message-receiver/card-message-receiver.h"
#define HALL_OF_FAME "src/include/scene/hall-of-fame/hall-of-fame.h"
#define HALL_OF_FAME_RECEIVER "src/include/utils/websocket-message-receiver/hall-of-fame-receiver.h"
#define HALL_OF_FAME_SENDER "src/include/utils/websocket-message-sender/hall-of-fame-sender.h"
#define PLAYER "src/include/scene/hall-of-fame/player.h"
#define LIBWEBSOCKETS_ADAPTER "src/include/utils/websocket-manager/libwebsockets/libwebsockets-adapter.h"
#define EMBSCRIPTEN_ADAPTER "src/include/utils/websocket-manager/emscripten/emscripten-adapter.h"
#define WEBSOCKET_ADAPTER "src/include/utils/websocket-manager/websocket-adapter.h"

#endif
