//
// Created by Sumn on 2024/1/18.
//

#include "audio_stream_callback.h"

namespace player {
    namespace audio {
        AudioPlayerStreamCallback::AudioPlayerStreamCallback(RingBuffer *ringBuffer) : mRingBuffer(
                ringBuffer) {}

        oboe::DataCallbackResult
        AudioPlayerStreamCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData,
                                            int32_t numFrames) {
            int32_t bytesToRead = numFrames * audioStream->getChannelCount() * sizeof(int16_t);
            int32_t bytesRead = mRingBuffer->read(static_cast<uint8_t *>(audioData), bytesToRead);
            if (bytesRead < bytesToRead) {
                return oboe::DataCallbackResult::Stop;
            } else {
                return oboe::DataCallbackResult::Continue;
            }
        }
    }
}