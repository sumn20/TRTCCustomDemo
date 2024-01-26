#include <jni.h>
#include "log_util.h"
#include "jni_util.h"
#include "java_basic_jni.h"
#include "trtc_cloud_defines.h"
#include "trtc_cloud.h"
#include "user_sig_generator.h"
#include "trtc_core.h"
#include "Oboe.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...)  \
    RETURN_TYPE NAME(JNIEnv *env, jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif
DEFINE_NATIVE_FUNC(void, enterRoom, const jstring userID, const jint roomID) {
    const char *pUserid = basic::jni::StringJni::Jstring2Cstring(env, userID).c_str();
    liteav::InitParams init_params;
    liteav::TRTCCloud::Initialize(init_params);
    liteav::EnterRoomParams params;
    params.sdk_app_id = SDKAPPID;
    params.user_id = pUserid;
    params.room_id = roomID;
    char sig[1024] = {0};
    size_t sig_size = 1024;
    GenerateUserSig(SDKAPPID, kSdkDemoKey, params.user_id, sig, &sig_size);
    params.user_sig = sig;
    params.scene = liteav::TRTC_SCENE_VIDEO_CALL;
    params.role = liteav::TRTC_ROLE_ANCHOR;
    TRTCCloudCore::GetInstance()->getTRTCCloud()->EnterRoom(params);
}
DEFINE_NATIVE_FUNC(void, exitRoom) {
    TRTCCloudCore::GetInstance()->getTRTCCloud()->ExitRoom();
    TRTCCloudCore::Destroy();

}
DEFINE_NATIVE_FUNC(jint, getCurrentAudioApi) {
    return TRTCCloudCore::GetInstance()->getCurrentAudioApi();
}
DEFINE_NATIVE_FUNC(void, changeAudioApi, jint audio_api) {
    TRTCCloudCore::GetInstance()->changeAudioApi((oboe::AudioApi) audio_api);
}
DEFINE_NATIVE_FUNC(void, startCapture) {
    TRTCCloudCore::GetInstance()->startCapture();

}
DEFINE_NATIVE_FUNC(void, stopCapture) {
    TRTCCloudCore::GetInstance()->stopCapture();
}
// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"enterRoom",          "(Ljava/lang/String;I)V", (void *) enterRoom},
        {"exitRoom",           "()V",                    (void *) exitRoom},
        {"getCurrentAudioApi", "()I",                    (void *) getCurrentAudioApi},
        {"changeAudioApi",     "(I)V",                   (void *) changeAudioApi},
        {"startCapture",       "()V",                    (void *) startCapture},
        {"stopCapture",        "()V",                    (void *) stopCapture},
};

// 注册 native 方法
int RegisterNativeMethodsForBusinessManager(JNIEnv *env) {
    return basic::jni::util::RegisterNativeMethods(env, "com/tencent/trtccustom/JNIManager",
                                                   gMethods,
                                                   sizeof(gMethods) / sizeof(gMethods[0]));
}
void test(JNIEnv *env) {
    jclass clazz = env->FindClass("com/tencent/trtccustom/JNIManager");
    env->GetMethodID(clazz, "enterRoom", "(Ljava/lang/String;I)V");
    env->GetMethodID(clazz, "getCurrentAudioApi", "()I");
    env->GetMethodID(clazz, "changeAudioApi", "(I)V");

}

#ifdef __cplusplus
}
#endif