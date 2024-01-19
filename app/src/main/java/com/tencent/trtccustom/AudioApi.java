package com.tencent.trtccustom;

public class AudioApi {

    public static final int Unspecified = 0;

    public static final int OpenSLES = 1;

    public static final int AAudio = 2;

    public static String apiToString(int api) {
        if (api == 1) {
            return "OpenSLES";

        } else if (api == 2) {
            return "AAudio";
        } else {
            return "Unspecified";
        }
    }
}
