#ifndef ANDROID_BRIDGE_H
#define ANDROID_BRIDGE_H

#include "src/paths.h"
#include MEMORY_MANAGER
#include <raylib.h>
#include <string.h>

#if defined(PLATFORM_ANDROID)
#include <android/native_activity.h>
#include <android_native_app_glue.h>

#include <jni.h>

typedef struct AndroidBridge AndroidBridge;

struct AndroidBridge {
    void (*showSoftKeyboard)(char *inputDestination, int maxLength);
    void (*hideSoftKeyboard)(void);
    void (*updateGuiTextBoxText)(const char *text);
    char *activeInputDestination;
    int activeInputMaxLength;
};

AndroidBridge *createAndroidBridge();

extern AndroidBridge *androidBridge;

extern struct android_app *androidApp;

struct android_app *GetAndroidApp(void);

#endif

#endif
