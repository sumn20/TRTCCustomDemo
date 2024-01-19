//
// Created by Sumn on 2024/1/18.
//

#ifndef TRTCCUSTOMDEMO_AUDIO_STREAM_CALLBACK_H
#define TRTCCUSTOMDEMO_AUDIO_STREAM_CALLBACK_H

#include "Oboe.h"
#include "audio_ring_buffer.h"
#include "log_util.h"

namespace player {
    namespace audio {
        class MyAudioStreamCallback : public oboe::AudioStreamCallback {
        public:
            MyAudioStreamCallback(RingBuffer *ringBuffer);
            oboe::DataCallbackResult
            onAudioReady(oboe::AudioStream *audioStream, void *audioData,
                         int32_t numFrames) override;

        private:
            RingBuffer *mRingBuffer;
        };
    }
}
#endif //TRTCCUSTOMDEMO_AUDIO_STREAM_CALLBACK_H
