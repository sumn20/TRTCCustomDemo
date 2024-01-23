//
// Created by Sumn on 2024/1/18.
//
#include "Oboe.h"
#include "audio_ring_buffer.h"
#include "audio_stream_callback.h"
#include "log_util.h"
#include "constant.h"

#ifndef TRTCCUSTOMDEMO_AUDIO_PLAYER_H
#define TRTCCUSTOMDEMO_AUDIO_PLAYER_H
using namespace oboe;
namespace player {
    namespace audio {
        class AudioPlayer {
        private:
            oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
            oboe::AudioStream *mStream;
            AudioPlayerStreamCallback *mCallBack;
            RingBuffer *mRingBuffer;

        public:
            basic::Result initAudioPlayerDevice();

            oboe::AudioStream *
            createAudioStream(oboe::AudioApi audioApi, oboe::AudioStreamCallback *callback);

            basic::Result start(const uint8_t *data, size_t dataSize);

            basic::Result changeAudioApi(oboe::AudioApi audioApi);

            oboe::AudioApi getCurrentAudioApi();

            int releaseAudioPlayerDevice();


        };
    }
}

#endif //TRTCCUSTOMDEMO_AUDIO_PLAYER_H
