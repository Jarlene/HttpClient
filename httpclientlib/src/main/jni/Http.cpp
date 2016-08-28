//
// Created by Jarlene on 2016/5/20.
//

#include <jni.h>
#include <stdio.h>
#include <assert.h>
#include "base/log.h"
#include "HttpManager.h"

#define LOG_TAG "Http"
#define MAIN_CLASS "com/baidu/music/httpclientlib/jni/HttpClientJNI"

static jclass mainClass;

static jstring httpRequest(JNIEnv* env, jclass clazz, jstring url) {
    jboolean isCopy = JNI_TRUE;
    const char * uri = env->GetStringUTFChars(url, &isCopy);
    char * result = HttpManager::getInstance()->doHttpGet(uri);
    jstring res = env->NewStringUTF(result);
    delete uri;
    return res;
}

static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {
    jclass clazz= env->FindClass(className);
    mainClass = (jclass) env->NewGlobalRef(clazz);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * native方法映射
 */
static JNINativeMethod nativeMethods[] = {
        { "doHttpRequest", "(Ljava/lang/String)Ljava/lang/String", (void*)httpRequest }
};





/**
 * 注册native方法
 */
static int registerNatives(JNIEnv* env) {
    if (!registerNativeMethods(env, MAIN_CLASS, nativeMethods,
                               sizeof(nativeMethods) / sizeof(nativeMethods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the HttpClientJNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);
    if (!registerNatives(env)) { //注册
        return -1;
    }
    /* success -- return valid version number */
    result = JNI_VERSION_1_6;

    return result;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnUnload");
    JNIEnv *env;
    int nJNIVersionOK = vm->GetEnv((void **)&env, JNI_VERSION_1_6) ;
    if (nJNIVersionOK == JNI_OK) {
        if(mainClass) {
            env->DeleteGlobalRef(mainClass);
        }
    }
}