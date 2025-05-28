#ifndef GLOBAL_WEBSOCKET_MANAGER_H
#define GLOBAL_WEBSOCKET_MANAGER_H

#include "src/paths.h"
#include <raylib.h>
#include <stdlib.h>
#include MEMORY_MANAGER
#include WEBSOCKET_ADAPTER
#include WEBSOCKET_SUBSCRIBER

#define USE_WSS 0
// Change this to your computer's local IP address (e.g. "192.168.1.100")
// You can find your IP address using:
// - On Linux: ip addr show
// - On Windows: ipconfig
// - On Mac: ifconfig
#define ADDRESS "localhost" // Your actual IP address or domain name
#define PORT 3000

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_ANDROID)
#include LIBWEBSOCKETS_ADAPTER
#elif defined(PLATFORM_WEB)
#include EMBSCRIPTEN_ADAPTER
#endif

#include <cjson/cJSON.h>

#define MAX_WEBSOCKET_SUBSCRIBERS 2

typedef struct GlobalWebsocketManager GlobalWebsocketManager;

struct GlobalWebsocketManager {
    WebsocketAdapter *adapter;
    void (*connectWebsocket)(GlobalWebsocketManager *manager, const char *address, int port);
    void (*sendWebsocketMessage)(GlobalWebsocketManager *manager, cJSON *json);
    void (*disconnectWebsocket)(GlobalWebsocketManager *manager);
    void (*destroy)(GlobalWebsocketManager *manager);
    void (*registerWebsocketSubscriber)(GlobalWebsocketManager *manager, WebsocketSubscriber *subscriber);
    void (*unregisterWebsocketSubscriber)(GlobalWebsocketManager *manager, WebsocketSubscriber *subscriber);
    WebsocketSubscriber *websocketSubscribers[MAX_WEBSOCKET_SUBSCRIBERS];
    size_t websocketSubscriberCount;
    bool (*isConnected)(GlobalWebsocketManager *manager);
    bool isDisconnecting;
};

GlobalWebsocketManager *createGlobalWebsocketManager();

extern GlobalWebsocketManager *globalWebsocketManager;

#endif
