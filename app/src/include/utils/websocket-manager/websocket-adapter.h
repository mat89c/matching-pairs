#ifndef WEBSOCKET_ADAPTER_H
#define WEBSOCKET_ADAPTER_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include <cjson/cJSON.h>
#include <raylib.h>

typedef struct WebsocketAdapter WebsocketAdapter;

struct WebsocketAdapter {
    void (*connectWebsocket)(WebsocketAdapter *adapter, const char *address, int port);
    void (*sendWebsocketMessage)(WebsocketAdapter *adapter, cJSON *json);
    void (*disconnectWebsocket)(WebsocketAdapter *adapter);
    void (*destroy)(WebsocketAdapter *adapter);
    bool isConnected;
};

#endif
