#include "src/paths.h"
#include <raylib.h>
#include WINDOW_MANAGER
#include MEMORY_MANAGER
#include SCENE_PROVIDER
#include SCENE_MANAGER
#include MAIN_MENU
#include MUSIC_MANAGER
#include OPTIONS
#define RAYGUI_IMPLEMENTATION
#include RAYGUI
#include GLOBAL_MOUSE
#include GLOBAL_WEBSOCKET_MANAGER

#if defined(PLATFORM_ANDROID)
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <jni.h>
extern struct android_app *app;

struct android_app *androidApp;
#endif

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#endif

MemoryManager *memoryManager = NULL;
SceneProvider *sceneProvider = NULL;
GlobalGameOptions *globalGameOptions = NULL;
GlobalMouse *globalMouse = NULL;
SceneManager *sceneManager = NULL;
MusicManager *musicManager = NULL;
GlobalWebsocketManager *globalWebsocketManager = NULL;

bool windowShouldClose = false;

void runSceneWrapper(void) {
    musicManager->updateMusic(musicManager);
    sceneManager->runScene(sceneProvider);
}

#if defined(PLATFORM_ANDROID)

static jobject featuresInstance = NULL;
struct android_app *GetAndroidApp(void);

JNIEnv *AttachCurrentThread(void) {
    JavaVM *vm = GetAndroidApp()->activity->vm;
    JNIEnv *env;

    (*vm)->AttachCurrentThread(vm, &env, NULL);
    return env;
}

void DetachCurrentThread(void) {
    JavaVM *vm = GetAndroidApp()->activity->vm;
    (*vm)->DetachCurrentThread(vm);
}

jobject GetNativeLoaderInstance(void) { return GetAndroidApp()->activity->clazz; }

jobject GetFeaturesInstance(void) {
    TraceLog(LOG_INFO, "GetFeaturesInstance: Starting");
    if (featuresInstance == NULL) {
        TraceLog(LOG_INFO, "GetFeaturesInstance: featuresInstance is NULL, creating new instance");
        JNIEnv *env = AttachCurrentThread();
        jobject nativeLoaderInstance = GetNativeLoaderInstance();
        TraceLog(LOG_INFO, "GetFeaturesInstance: Got nativeLoaderInstance");

        jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
        jmethodID getFeaturesMethod =
            (*env)->GetMethodID(env, nativeLoaderClass, "getFeatures", "()Lcom/mat89c/matching_pairs/Features;");

        if (getFeaturesMethod == NULL) {
            TraceLog(LOG_ERROR, "GetFeaturesInstance: getFeatures method not found!");
            return NULL;
        }

        TraceLog(LOG_INFO, "GetFeaturesInstance: Calling getFeatures method");
        jobject localFeaturesInstance = (*env)->CallObjectMethod(env, nativeLoaderInstance, getFeaturesMethod);
        featuresInstance = (*env)->NewGlobalRef(env, localFeaturesInstance);
        TraceLog(LOG_INFO, "GetFeaturesInstance: Created new features instance");

        DetachCurrentThread();
    } else {
        TraceLog(LOG_INFO, "GetFeaturesInstance: Using existing features instance");
    }

    return featuresInstance;
}

void ShowSoftKeyboard(void) {
    TraceLog(LOG_INFO, "ShowSoftKeyboard: Starting");
    JNIEnv *env = AttachCurrentThread();
    jobject nativeLoaderInstance = GetNativeLoaderInstance();
    TraceLog(LOG_INFO, "ShowSoftKeyboard: Got native loader instance");

    jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
    jmethodID method = (*env)->GetMethodID(env, nativeLoaderClass, "showKeyboard", "()V");

    if (method == NULL) {
        TraceLog(LOG_ERROR, "ShowSoftKeyboard: showKeyboard method not found!");
        DetachCurrentThread();
        return;
    }

    TraceLog(LOG_INFO, "ShowSoftKeyboard: Calling showKeyboard method");
    (*env)->CallVoidMethod(env, nativeLoaderInstance, method);
    DetachCurrentThread();
    TraceLog(LOG_INFO, "ShowSoftKeyboard: Completed");
}

void HideSoftKeyboard(void) {
    TraceLog(LOG_INFO, "HideSoftKeyboard: Starting");
    JNIEnv *env = AttachCurrentThread();
    jobject nativeLoaderInstance = GetNativeLoaderInstance();
    TraceLog(LOG_INFO, "HideSoftKeyboard: Got native loader instance");

    jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
    jmethodID method = (*env)->GetMethodID(env, nativeLoaderClass, "hideKeyboard", "()V");

    if (method == NULL) {
        TraceLog(LOG_ERROR, "HideSoftKeyboard: hideKeyboard method not found!");
        DetachCurrentThread();
        return;
    }

    TraceLog(LOG_INFO, "HideSoftKeyboard: Calling hideKeyboard method");
    (*env)->CallVoidMethod(env, nativeLoaderInstance, method);
    DetachCurrentThread();
    TraceLog(LOG_INFO, "HideSoftKeyboard: Completed");
}
#endif

int main(void) {
#if defined(PLATFORM_DESKTOP)
    ChangeDirectory(TextFormat("%s", GetApplicationDirectory()));
#endif
    TraceLog(LOG_INFO, "Starting application initialization");
    createWindow();
    InitAudioDevice();

    TraceLog(LOG_INFO, "Creating managers");
    memoryManager = createMemoryManager();
    sceneProvider = createSceneProvider();
    globalGameOptions = createGlobalGameOptions();
    globalMouse = createGlobalMouse();
    sceneManager = createSceneManager();
    globalWebsocketManager = createGlobalWebsocketManager();
    MainMenu *mainMenu = createMainMenu();
    sceneProvider->setScene((Scene *)mainMenu, sceneProvider);

    TraceLog(LOG_INFO, "Initializing music manager");
    musicManager = createMusicManager(ASSETS_PATH_PREFIX "audio/music.ogg");
    musicManager->playMusic(musicManager);

    TraceLog(LOG_INFO, "Connecting to websocket");
    globalWebsocketManager->connectWebsocket(globalWebsocketManager, ADDRESS, PORT);
    TraceLog(LOG_INFO, "Websocket connection initiated");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(runSceneWrapper, 0, 1);
#else
    TraceLog(LOG_INFO, "Entering main game loop");
    while (!WindowShouldClose() && !windowShouldClose) {
        runSceneWrapper();
    }
    TraceLog(LOG_INFO, "Exiting main game loop");
#endif

    TraceLog(LOG_INFO, "Cleaning up resources");
    musicManager->destroy(musicManager);
    sceneProvider->scene->destroy(sceneProvider->scene);

    globalWebsocketManager->disconnectWebsocket(globalWebsocketManager);

    destroyMemoryManager(memoryManager);

    CloseAudioDevice();
    CloseWindow();
    TraceLog(LOG_INFO, "Application shutdown complete");

    return 0;
}
