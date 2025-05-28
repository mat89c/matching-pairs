#include "src/paths.h"
#include GLOBAL_WEBSOCKET_MANAGER

static void connectWebsocket(GlobalWebsocketManager *manager, const char *address, int port) {
    TraceLog(LOG_INFO, "Connecting websocket to %s:%d", address, port);
    WebsocketAdapter *adapter = manager->adapter;
    adapter->connectWebsocket(adapter, address, port);
    TraceLog(LOG_INFO, "Websocket connection initiated");
}

static bool isConnected(GlobalWebsocketManager *manager) { return manager->adapter->isConnected; }

static void sendWebsocketMessage(GlobalWebsocketManager *manager, cJSON *json) {
    WebsocketAdapter *adapter = manager->adapter;
    adapter->sendWebsocketMessage(adapter, json);
}

static void disconnectWebsocket(GlobalWebsocketManager *manager) {
    if (manager->isDisconnecting) {
        TraceLog(LOG_INFO, "Websocket disconnection already in progress");
        return;
    }

    manager->isDisconnecting = true;
    TraceLog(LOG_INFO, "Disconnecting websocket");
    WebsocketAdapter *adapter = manager->adapter;
    adapter->disconnectWebsocket(adapter);
    TraceLog(LOG_INFO, "Websocket disconnected");
    manager->isDisconnecting = false;
}

static void destroy(GlobalWebsocketManager *manager) {
    TraceLog(LOG_INFO, "Destroying websocket manager");
    if (manager->isConnected(manager)) {
        manager->disconnectWebsocket(manager);
    }

    WebsocketAdapter *adapter = manager->adapter;
    adapter->destroy(adapter);
    TraceLog(LOG_INFO, "Websocket manager destroyed");
}

static void registerWebsocketSubscriber(GlobalWebsocketManager *manager, WebsocketSubscriber *websocketSubscriber) {
    TraceLog(LOG_INFO, "Registering websocket subscriber");
    if (websocketSubscriber == NULL) {
        TraceLog(LOG_ERROR, "Cannot register NULL websocket subscriber");
        exit(EXIT_FAILURE);
    }

    if (manager->websocketSubscriberCount >= MAX_WEBSOCKET_SUBSCRIBERS) {
        TraceLog(LOG_ERROR, "Maximum number of websocket subscribers reached (%d)", MAX_WEBSOCKET_SUBSCRIBERS);
        exit(EXIT_FAILURE);
    }

    manager->websocketSubscribers[manager->websocketSubscriberCount] = websocketSubscriber;
    manager->websocketSubscriberCount++;
    TraceLog(LOG_INFO, "Websocket subscriber registered, total subscribers: %d", manager->websocketSubscriberCount);
}

static void unregisterWebsocketSubscriber(GlobalWebsocketManager *manager, WebsocketSubscriber *websocketSubscriber) {
    TraceLog(LOG_INFO, "Unregistering websocket subscriber");
    for (size_t i = 0; i < manager->websocketSubscriberCount; i++) {
        if (manager->websocketSubscribers[i] == websocketSubscriber) {
            for (size_t j = i; j < manager->websocketSubscriberCount - 1; j++) {
                manager->websocketSubscribers[j] = manager->websocketSubscribers[j + 1];
            }
            manager->websocketSubscriberCount--;
            TraceLog(LOG_INFO, "Websocket subscriber unregistered, remaining subscribers: %d",
                     manager->websocketSubscriberCount);
            break;
        }
    }
}

GlobalWebsocketManager *createGlobalWebsocketManager() {
    TraceLog(LOG_INFO, "Creating global websocket manager");
    GlobalWebsocketManager *manager = malloc(sizeof(GlobalWebsocketManager));
    if (NULL == manager) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for global websocket manager");
        exit(EXIT_FAILURE);
    }

    manager->connectWebsocket = connectWebsocket;
    manager->sendWebsocketMessage = sendWebsocketMessage;
    manager->disconnectWebsocket = disconnectWebsocket;
    manager->destroy = destroy;
    manager->registerWebsocketSubscriber = registerWebsocketSubscriber;
    manager->unregisterWebsocketSubscriber = unregisterWebsocketSubscriber;
    memset(manager->websocketSubscribers, 0, sizeof(manager->websocketSubscribers));
    manager->websocketSubscriberCount = 0;
    manager->isConnected = isConnected;
    manager->isDisconnecting = false;

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_ANDROID)
    TraceLog(LOG_INFO, "Creating libwebsockets adapter");
    LibwebsocketsAdapter *libwebsocketsAdapter = createLibwebsocketsAdapter();
    manager->adapter = &libwebsocketsAdapter->base;
#elif defined(PLATFORM_WEB)
    TraceLog(LOG_INFO, "Creating emscripten adapter");
    EmscriptenAdapter *emscriptenAdapter = createEmscriptenAdapter();
    manager->adapter = &emscriptenAdapter->base;
#else
    TraceLog(LOG_ERROR, "Unsupported platform");
    exit(EXIT_FAILURE);
#endif

    memoryManager->addObject(manager);
    TraceLog(LOG_INFO, "Global websocket manager created successfully");
    return manager;
}
