#include "src/paths.h"
#include ANDROID_BRIDGE

#if defined(PLATFORM_ANDROID)
static jobject featuresInstance = NULL;

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
    if (featuresInstance == NULL) {
        JNIEnv *env = AttachCurrentThread();
        jobject nativeLoaderInstance = GetNativeLoaderInstance();

        jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
        jmethodID getFeaturesMethod =
            (*env)->GetMethodID(env, nativeLoaderClass, "getFeatures", "()Lcom/mat89c/matching_pairs/Features;");

        if (getFeaturesMethod == NULL) {
            TraceLog(LOG_ERROR, "GetFeaturesInstance: getFeatures method not found!");
            return NULL;
        }

        jobject localFeaturesInstance = (*env)->CallObjectMethod(env, nativeLoaderInstance, getFeaturesMethod);
        featuresInstance = (*env)->NewGlobalRef(env, localFeaturesInstance);

        DetachCurrentThread();
    }

    return featuresInstance;
}

JNIEXPORT void JNICALL Java_com_mat89c_matching_1pairs_NativeLoader_nativeOnTextInput(JNIEnv *env, jobject thiz,
                                                                                      jstring text) {
    const char *nativeText = (*env)->GetStringUTFChars(env, text, 0);
    if (NULL == androidBridge) {
        TraceLog(LOG_ERROR, "nativeOnTextInput: androidBridge is NULL");
        return;
    }

    androidBridge->updateGuiTextBoxText(nativeText);

    (*env)->ReleaseStringUTFChars(env, text, nativeText);
}

static void showSoftKeyboard(char *inputDestination, int maxLength) {
    JNIEnv *env = AttachCurrentThread();
    jobject nativeLoaderInstance = GetNativeLoaderInstance();

    jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
    jmethodID method = (*env)->GetMethodID(env, nativeLoaderClass, "showKeyboard", "()V");

    if (method == NULL) {
        TraceLog(LOG_ERROR, "ShowSoftKeyboard: showKeyboard method not found!");
        DetachCurrentThread();
        return;
    }

    androidBridge->activeInputDestination = inputDestination;
    androidBridge->activeInputMaxLength = maxLength;

    (*env)->CallVoidMethod(env, nativeLoaderInstance, method);
    DetachCurrentThread();
}

static void hideSoftKeyboard(void) {
    jobject nativeLoaderInstance = GetNativeLoaderInstance();

    jclass nativeLoaderClass = (*env)->GetObjectClass(env, nativeLoaderInstance);
    jmethodID method = (*env)->GetMethodID(env, nativeLoaderClass, "hideKeyboard", "()V");

    if (method == NULL) {
        TraceLog(LOG_ERROR, "HideSoftKeyboard: hideKeyboard method not found!");
        DetachCurrentThread();
        return;
    }

    androidBridge->activeInputDestination = NULL;
    androidBridge->activeInputMaxLength = 0;

    (*env)->CallVoidMethod(env, nativeLoaderInstance, method);
    DetachCurrentThread();
}

static void updateGuiTextBoxText(const char *text) {
    if (androidBridge->activeInputDestination == NULL) {
        return;
    }

    int currentLength = strlen(androidBridge->activeInputDestination);
    int textLength = strlen(text);

    if (textLength == 1 && text[0] == '\b') {
        if (currentLength > 0) {
            androidBridge->activeInputDestination[currentLength - 1] = '\0';
        }
    } else if (textLength > 1) {
        strncpy(androidBridge->activeInputDestination, text, androidBridge->activeInputMaxLength - 1);
        androidBridge->activeInputDestination[androidBridge->activeInputMaxLength - 1] = '\0';
    } else if (currentLength < androidBridge->activeInputMaxLength - 1) {
        androidBridge->activeInputDestination[currentLength] = text[0];
        androidBridge->activeInputDestination[currentLength + 1] = '\0';
    }
}

AndroidBridge *createAndroidBridge(void) {
    if (androidBridge == NULL) {
        androidBridge = (AndroidBridge *)malloc(sizeof(AndroidBridge));
        if (androidBridge == NULL) {
            exit(EXIT_FAILURE);
        }

        androidBridge->showSoftKeyboard = showSoftKeyboard;
        androidBridge->hideSoftKeyboard = hideSoftKeyboard;
        androidBridge->updateGuiTextBoxText = updateGuiTextBoxText;
        androidBridge->activeInputDestination = NULL;
        androidBridge->activeInputMaxLength = 0;

        memoryManager->addObject(androidBridge);
    }
    return androidBridge;
}

#endif
