#include "src/paths.h"
#include EMBSCRIPTEN_ADAPTER
#include MEMORY_MANAGER
#include WEBSOCKET_ADAPTER

static EM_BOOL onOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData) {
    printf("[WebSocket] Connected!\n");
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)userData;
    adapter->base.isConnected = true;
    return EM_TRUE;
}

static EM_BOOL onClose(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData) {
    printf("[WebSocket] Disconnected!\n");
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)userData;
    adapter->base.isConnected = false;
    return EM_TRUE;
}

static EM_BOOL onError(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData) {
    printf("[WebSocket] Error occurred!\n");
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)userData;
    adapter->base.isConnected = false;
    return EM_TRUE;
}

static EM_BOOL onMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData) {
    if (e->isText) {
        cJSON *json = cJSON_Parse((const char *)e->data);
        if (json != NULL) {
            for (size_t i = 0; i < globalWebsocketManager->websocketSubscriberCount; i++) {
                if (globalWebsocketManager->websocketSubscribers[i] != NULL) {
                    globalWebsocketManager->websocketSubscribers[i]->subscribe(
                        globalWebsocketManager->websocketSubscribers[i], json);
                }
            }

            cJSON_Delete(json);
        }
    }
    return EM_TRUE;
}

static void connectWebsocket(WebsocketAdapter *abstractAdapter, const char *address, int port) {
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)abstractAdapter;

    if (!emscripten_websocket_is_supported()) {
        printf("[WebSocket] Websockets not supported in this environment.\n");
        exit(EXIT_FAILURE);
    }

    char url[256];
    if (1 == USE_WSS) {
        snprintf(url, sizeof(url), "wss://%s:%d/game", address, port);
    } else {
        snprintf(url, sizeof(url), "ws://%s:%d/game", address, port);
    }

    EmscriptenWebSocketCreateAttributes attributes = {
        .url = url,
        .protocols = NULL,
        .createOnMainThread = EM_TRUE,
    };

    adapter->socket = emscripten_websocket_new(&attributes);

    if (adapter->socket <= 0) {
        printf("[WebSocket] Failed to create socket.\n");
        return;
    }

    emscripten_websocket_set_onopen_callback(adapter->socket, adapter, onOpen);
    emscripten_websocket_set_onclose_callback(adapter->socket, adapter, onClose);
    emscripten_websocket_set_onerror_callback(adapter->socket, adapter, onError);
    emscripten_websocket_set_onmessage_callback(adapter->socket, adapter, onMessage);
}

static void sendWebsocketMessage(WebsocketAdapter *abstractAdapter, cJSON *json) {
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)abstractAdapter;
    if (!adapter->base.isConnected) {
        printf("[WebSocket] Not connected. Cannot send message.\n");
        return;
    }

    char *msg = cJSON_PrintUnformatted(json);
    emscripten_websocket_send_utf8_text(adapter->socket, msg);
    free(msg);
}

static void disconnectWebsocket(WebsocketAdapter *abstractAdapter) {
    EmscriptenAdapter *adapter = (EmscriptenAdapter *)abstractAdapter;
    emscripten_websocket_close(adapter->socket, 1000, "Disconnecting");
}

static void destroy(WebsocketAdapter *abstractAdapter) {}

EmscriptenAdapter *createEmscriptenAdapter() {
    EmscriptenAdapter *adapter = malloc(sizeof(EmscriptenAdapter));
    if (NULL == adapter) {
        exit(EXIT_FAILURE);
    }

    adapter->base.connectWebsocket = connectWebsocket;
    adapter->base.sendWebsocketMessage = sendWebsocketMessage;
    adapter->base.disconnectWebsocket = disconnectWebsocket;
    adapter->base.destroy = destroy;
    adapter->base.isConnected = false;
    adapter->socket = 0;

    memoryManager->addObject(adapter);

    return adapter;
}
