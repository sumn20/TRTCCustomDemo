//
// Created by Sumn on 2024/1/24.
//

#ifndef TRTCCUSTOMDEMO_AUDIO_CAPTURE_STREAM_CALLBACK_H
#define TRTCCUSTOMDEMO_AUDIO_CAPTURE_STREAM_CALLBACK_H

#include "Oboe.h"
#include "audio_ring_buffer.h"
#include "trtc_cloud.h"
#include "constant.h"
#include "log_util.h"
namespace capture {
    namespace audio {
        class AudioCaptureStreamCallback
                : public oboe::AudioStreamCallback {
        public:
            AudioCaptureStreamCallback(RingBuffer
                                       *ringBuffer, liteav::TRTCCloud *cloud);

            oboe::DataCallbackResult
            onAudioReady(oboe::AudioStream *oboeStream, void *audioData,
                         int32_t numFrames) override;

        private:
            RingBuffer *mRingBuffer;
            liteav::TRTCCloud *pCloud;
        };
    }
}


#endif //TRTCCUSTOMDEMO_AUDIO_CAPTURE_STREAM_CALLBACK_H
