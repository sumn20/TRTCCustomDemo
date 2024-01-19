//
// Created by EDY on 2022/8/22.
//

#include <android/log.h>
#include <jni.h>

//日志打印
#include "log_util.h"

// Java 基础数据类型
#include "java_basic_jni.h"
#include "jni_helper.h"


#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_INITIDS_RESULT(success, class_name) \
    if(false == (success)) {                      \
        LOGCATE("%s InitIDs failed",class_name);     \
    }

extern int RegisterNativeMethodsForBusinessManager(JNIEnv *env);




static void InitJNI(JNIEnv *env) {
    // Java 基础数据类型
    bool success = basic::jni::IntegerJni::Init(env);
    CHECK_INITIDS_RESULT(success, "IntegerJni")

    success = basic::jni::LongJni::Init(env);
    CHECK_INITIDS_RESULT(success, "LongJni")

    success = basic::jni::StringJni::Init(env);
    CHECK_INITIDS_RESULT(success, "StringJni")

    success = basic::jni::ArrayListJni::Init(env);
    CHECK_INITIDS_RESULT(success, "ArrayListJni")

    success = basic::jni::HashMapJni::Init(env);
    CHECK_INITIDS_RESULT(success, "HashMapJni")
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    basic::jni::JniHelper::Init(vm);

    jint result = JNI_ERR;
    basic::jni::ScopedJEnv scopedJEnv;
    JNIEnv *env = scopedJEnv.GetEnv();
    if (NULL == env) {
        __android_log_print(ANDROID_LOG_ERROR, "liteav",
                            " ############### invalid JNIEnv ############### ");
        return result;
    }
    LOGCATD("jvm_wrapper.cpp JNI_OnLoad | env = %p", env);
    InitJNI(env);

    if (JNI_TRUE != RegisterNativeMethodsForBusinessManager(env)) {
        return result;
    }



#ifdef __arm__
    __android_log_print(ANDROID_LOG_INFO, "liteav",
                        " ############### TRTCSDK version arm32 ############### ");
#endif

#ifdef __aarch64__
    __android_log_print(ANDROID_LOG_INFO, "liteav",
                        " ################ TRTCSDK version arm64 ############### ");
#endif

    return JNI_VERSION_1_6;
}

//JNIEXPORT void JNI_OnUnload(JavaVM *aJvm, void *aReserved){}

#ifdef __cplusplus
}
#endif