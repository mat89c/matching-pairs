#include "src/paths.h"
#include WEBSOCKET_SUBSCRIBER

static void subscribe(WebsocketSubscriber *subscriber, cJSON *json) { subscriber->callback(subscriber->self, json); }

WebsocketSubscriber *createWebsocketSubscriber(void *self, WebsocketMessageCallback callback) {
    WebsocketSubscriber *subscriber = malloc(sizeof(WebsocketSubscriber));
    if (NULL == subscriber) {
        printf("Failed to allocate memory for websocket subscriber\n");
        exit(EXIT_FAILURE);
    }

    subscriber->subscribe = subscribe;
    subscriber->callback = callback;
    subscriber->self = self;

    memoryManager->addObject(subscriber);

    return subscriber;
}
