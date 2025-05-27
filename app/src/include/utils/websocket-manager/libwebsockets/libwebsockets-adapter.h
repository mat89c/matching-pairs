#ifndef LIBWEBSOCKETS_ADAPTER_H
#define LIBWEBSOCKETS_ADAPTER_H

#include "src/paths.h"
#if defined(PLATFORM_ANDROID)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include MEMORY_MANAGER
#include WEBSOCKET_SUBSCRIBER
#include GLOBAL_WEBSOCKET_MANAGER
#include WEBSOCKET_ADAPTER
#include <cjson/cJSON.h>
#include <libwebsockets.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_MESSAGE_LENGTH 256
#define MATCHIN_PAIRS_PROTOCOL "matching-pairs-protocol"
#define MAX_WEBSOCKET_SUBSCRIBERS 2
#define MAX_BUFFER_SIZE 4096

typedef struct LibwebsocketsAdapter LibwebsocketsAdapter;

struct LibwebsocketsAdapter {
    WebsocketAdapter base;
    struct lws_context *context;
    struct lws *wsi;
    pthread_t service_thread;
    bool is_running;
    void *(*websocketServiceThread)(void *arg);
    int (*callback)(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
};

LibwebsocketsAdapter *createLibwebsocketsAdapter();

extern LibwebsocketsAdapter *libwebsocketsAdapter;

#endif
