#ifndef WEBSOCKET_SUBSCRIBER_H
#define WEBSOCKET_SUBSCRIBER_H

#include "src/paths.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include MEMORY_MANAGER

typedef void (*WebsocketMessageCallback)(void *self, cJSON *json);

typedef struct WebsocketSubscriber WebsocketSubscriber;

struct WebsocketSubscriber {
    void (*subscribe)(WebsocketSubscriber *subscriber, cJSON *json);
    WebsocketMessageCallback callback;
    void *self;
};

WebsocketSubscriber *createWebsocketSubscriber(void *self, WebsocketMessageCallback callback);
#endif
