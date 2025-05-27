#ifndef EMBSCRIPTEN_ADAPTER_H
#define EMBSCRIPTEN_ADAPTER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include WEBSOCKET_ADAPTER
#include GLOBAL_WEBSOCKET_MANAGER
#include <cjson/cJSON.h>
#include <emscripten.h>
#include <emscripten/websocket.h>
#include <stdio.h>

typedef struct EmscriptenAdapter {
    WebsocketAdapter base;
    EMSCRIPTEN_WEBSOCKET_T socket;
} EmscriptenAdapter;

EmscriptenAdapter *createEmscriptenAdapter();

#endif
