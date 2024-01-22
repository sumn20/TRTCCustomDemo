//
// Created by Sumn on 2024/1/19.
//

#ifndef TRTCCUSTOMDEMO_CONSTANT_H
#define TRTCCUSTOMDEMO_CONSTANT_H

#include <cstdint>

namespace basic {
    class AudioConfig {
    public:
        static int sampleRate;
        static int channelCount;

        static void setSampleRate(int rate) {
            sampleRate = rate;
        }

        static void setChannelCount(int count) {
            channelCount = count;
        }
    };

    int AudioConfig::sampleRate = 48000;
    int AudioConfig::channelCount = 2;

    enum class Result : int32_t {
        OK = 0,
        ErrorAudioInit = -100,
        ErrorAudioPlayer = -101,
        ErrorAudioChange = -102,


    };
}
#endif //TRTCCUSTOMDEMO_CONSTANT_H
