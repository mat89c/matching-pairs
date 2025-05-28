#include "src/paths.h"
#include LIBWEBSOCKETS_ADAPTER

static void *websocketServiceThread(void *arg) {
    LibwebsocketsAdapter *adapter = (LibwebsocketsAdapter *)arg;
    while (adapter->is_running) {
        lws_service(adapter->context, 50);
    }
    return NULL;
}

static void connectWebsocket(WebsocketAdapter *abstractAdapter, const char *address, int port) {
    TraceLog(LOG_INFO, "Connecting websocket to %s:%d", address, port);
    LibwebsocketsAdapter *adapter = (LibwebsocketsAdapter *)abstractAdapter;
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.user = adapter;
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = (struct lws_protocols[]){{MATCHIN_PAIRS_PROTOCOL, adapter->callback, 0, MAX_MESSAGE_LENGTH},
                                              {NULL, NULL, 0, 0}};
    info.gid = -1;
    info.uid = -1;

    if (1 == USE_WSS) {
        info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        info.client_ssl_cert_filepath = NULL;
        info.client_ssl_private_key_filepath = NULL;
        info.client_ssl_ca_filepath = NULL;
        info.ssl_private_key_filepath = NULL;
        info.ssl_cert_filepath = NULL;
        info.ssl_ca_filepath = NULL;
    }

    TraceLog(LOG_INFO, "Creating websocket context");
    adapter->context = lws_create_context(&info);
    if (!adapter->context) {
        TraceLog(LOG_ERROR, "Cannot create websocket context");
        exit(EXIT_FAILURE);
    }

    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = adapter->context;
    ccinfo.address = address;
    ccinfo.port = port;
    ccinfo.path = "/game";
    ccinfo.host = address;
    ccinfo.protocol = MATCHIN_PAIRS_PROTOCOL;

    if (1 == USE_WSS) {
        ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK | LCCSCF_ALLOW_INSECURE |
                                LCCSCF_ALLOW_EXPIRED | LCCSCF_ALLOW_SELFSIGNED;
    }

    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_CLIENT | LLL_HEADER, NULL);

    TraceLog(LOG_INFO, "Connecting to websocket server");
    adapter->wsi = lws_client_connect_via_info(&ccinfo);
    if (!adapter->wsi) {
        TraceLog(LOG_ERROR, "Cannot connect to websocket server");
        TraceLog(LOG_ERROR, "Last error: %s", strerror(errno));
        lws_context_destroy(adapter->context);
        exit(EXIT_FAILURE);
    }

    lws_set_opaque_user_data(adapter->wsi, adapter);

    adapter->is_running = true;
    TraceLog(LOG_INFO, "Creating websocket service thread");
    if (pthread_create(&adapter->service_thread, NULL, adapter->websocketServiceThread, adapter) != 0) {
        TraceLog(LOG_ERROR, "Cannot create websocket service thread");
        lws_context_destroy(adapter->context);
        exit(EXIT_FAILURE);
    }
    TraceLog(LOG_INFO, "Websocket connection initiated");
}

static int callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    LibwebsocketsAdapter *adapter = (LibwebsocketsAdapter *)lws_context_user(lws_get_context(wsi));

    static char buffer[MAX_BUFFER_SIZE];
    static size_t buffer_pos = 0;

    switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        buffer_pos = 0;
        TraceLog(LOG_INFO, "WebSocket connection established");
        adapter->base.isConnected = true;

        break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
        if (buffer_pos + len >= MAX_BUFFER_SIZE) {
            TraceLog(LOG_ERROR, "Buffer overflow, resetting buffer");
            buffer_pos = 0;
            return -1;
        }

        memcpy(buffer + buffer_pos, in, len);
        buffer_pos += len;

        if (lws_is_final_fragment(wsi)) {
            buffer[buffer_pos] = '\0';

            cJSON *json = cJSON_Parse(buffer);
            if (json != NULL) {
                for (size_t i = 0; i < globalWebsocketManager->websocketSubscriberCount; i++) {
                    if (globalWebsocketManager->websocketSubscribers[i] != NULL) {
                        globalWebsocketManager->websocketSubscribers[i]->subscribe(
                            globalWebsocketManager->websocketSubscribers[i], json);
                    }
                }

                cJSON_Delete(json);
            }

            buffer_pos = 0;
        }
        break;
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        TraceLog(LOG_ERROR, "WebSocket connection error");
        TraceLog(LOG_ERROR, "Error details: %s", in ? (char *)in : "No error details available");
        TraceLog(LOG_ERROR, "Last error: %s", strerror(errno));
        TraceLog(LOG_ERROR, "WebSocket state: %d", lws_http_client_http_response(wsi));
        adapter->base.isConnected = false;
        buffer_pos = 0;
        break;
    case LWS_CALLBACK_CLOSED:
        buffer_pos = 0;
        break;
    default:
        break;
    }
    return 0;
}

void sendWebsocketMessage(WebsocketAdapter *abstractAdapter, cJSON *json) {
    LibwebsocketsAdapter *adapter = (LibwebsocketsAdapter *)abstractAdapter;

    if (!adapter->wsi) {
        TraceLog(LOG_ERROR, "Cannot send message - websocket instance is NULL");
        return;
    }

    char *message = cJSON_Print(json);
    if (!message) {
        TraceLog(LOG_ERROR, "Cannot print message");
        exit(EXIT_FAILURE);
    }

    unsigned char buf[LWS_PRE + MAX_MESSAGE_LENGTH];
    size_t len = strlen(message);

    if (len > MAX_MESSAGE_LENGTH) {
        TraceLog(LOG_ERROR, "Message is too long (%d > %d)", len, MAX_MESSAGE_LENGTH);
        free(message);
        exit(EXIT_FAILURE);
    }

    memcpy(buf + LWS_PRE, message, len);

    int result = lws_write(adapter->wsi, buf + LWS_PRE, len, LWS_WRITE_TEXT);
    free(message);

    if (result == -1) {
        TraceLog(LOG_ERROR, "Cannot send message - lws_write returned -1");
        return;
    }
}

static void disconnectWebsocket(WebsocketAdapter *abstractAdapter) {
    TraceLog(LOG_INFO, "Disconnecting websocket");
    LibwebsocketsAdapter *adapter = (LibwebsocketsAdapter *)abstractAdapter;

    if (!adapter->context) {
        TraceLog(LOG_WARNING, "Websocket already disconnected");
        return;
    }

    adapter->base.isConnected = false;
    adapter->is_running = false;
    pthread_join(adapter->service_thread, NULL);
    lws_context_destroy(adapter->context);
    adapter->context = NULL;
    adapter->wsi = NULL;
    TraceLog(LOG_INFO, "Websocket disconnected");
}

static void destroyLibwebsocketsAdapter(WebsocketAdapter *abstractAdapter) {
    TraceLog(LOG_INFO, "Destroying libwebsockets adapter");
}

LibwebsocketsAdapter *createLibwebsocketsAdapter() {
    TraceLog(LOG_INFO, "Creating libwebsockets adapter");
    LibwebsocketsAdapter *adapter = malloc(sizeof(LibwebsocketsAdapter));
    if (NULL == adapter) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for libwebsockets adapter");
        exit(EXIT_FAILURE);
    }

    adapter->base.connectWebsocket = connectWebsocket;
    adapter->base.sendWebsocketMessage = sendWebsocketMessage;
    adapter->base.disconnectWebsocket = disconnectWebsocket;
    adapter->base.destroy = destroyLibwebsocketsAdapter;
    adapter->base.isConnected = false;
    adapter->context = NULL;
    adapter->wsi = NULL;
    adapter->is_running = false;
    adapter->websocketServiceThread = websocketServiceThread;
    adapter->callback = callback;

    memoryManager->addObject(adapter);
    TraceLog(LOG_INFO, "Libwebsockets adapter created successfully");
    return adapter;
}
