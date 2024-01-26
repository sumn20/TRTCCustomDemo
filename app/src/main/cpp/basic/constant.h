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
        Error=-1,
        ErrorNull = -99,
        ErrorAudioInit = -100,
        ErrorAudioPlayer = -101,
        ErrorAudioChange = -102,
        ErrorAudioVolumeType = -103,
    };

    enum class VolumeType {
        MEDIA = 1,
        VOIP = 2,
    };
}
#endif //TRTCCUSTOMDEMO_CONSTANT_H
