//
// Created by Sumn on 2024/1/19.
//

#ifndef TRTCCUSTOMDEMO_CONSTANT_H
#define TRTCCUSTOMDEMO_CONSTANT_H

#include <cstdint>

namespace basic {
    struct AudioConfig {
        AudioConfig() :
                sampleRate(48000),
                channelCount(2) {}

        int sampleRate;
        int channelCount;
    };

    enum class Result : int32_t {
        OK = 0,
        ErrorAudioInit = -100,
        ErrorAudioPlayer = -101,
        ErrorAudioChange = -102,


    };
}
#endif //TRTCCUSTOMDEMO_CONSTANT_H
