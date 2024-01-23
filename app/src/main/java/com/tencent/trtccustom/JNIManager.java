package com.tencent.trtccustom;

public class JNIManager {
    private final static String TAG = JNIManager.class.getSimpleName();

    private static class JNIManagerHolder {
        private static final JNIManager jniManager = new JNIManager();
    }

    public static JNIManager getInstance() {
        return JNIManagerHolder.jniManager;
    }

    protected JNIManager() {

    }

    static {
        System.loadLibrary("trtccustom");
    }

    native void enterRoom(String userID, int roomID);

    native void exitRoom();

    native void changeAudioApi(int audioApi);

    native int getCurrentAudioApi();

    native void startCapture();
    native void stopCapture();
}
