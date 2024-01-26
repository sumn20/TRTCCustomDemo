//
// Created by Sumn on 2024/1/24.
//

#include "audio_capture_stream_callback.h"


namespace capture {
    namespace audio {

        AudioCaptureStreamCallback::AudioCaptureStreamCallback(RingBuffer *ringBuffer,
                                                               liteav::TRTCCloud *cloud)
                : mRingBuffer(ringBuffer), pCloud(cloud) {

        }

        oboe::DataCallbackResult
        AudioCaptureStreamCallback::onAudioReady(oboe::AudioStream *oboeStream, void *audioData,
                                                 int32_t numFrames) {
            mRingBuffer->write(static_cast<uint8_t *>(audioData),
                               numFrames * oboeStream->getBytesPerFrame());
            auto framesFor20ms = static_cast<int32_t>(oboeStream->getSampleRate() * 0.02);
            int32_t sizeInBytes = framesFor20ms * oboeStream->getBytesPerFrame();
            if (mRingBuffer->getSize() >= sizeInBytes) {
                uint8_t buffer[sizeInBytes];
                mRingBuffer->read(buffer, sizeInBytes);
                basic::AudioConfig audioConfig;
                liteav::AudioFrame audioFrame;
                audioFrame.channels = audioConfig.channelCount;
                audioFrame.sample_rate = audioConfig.sampleRate;
                audioFrame.SetData(static_cast<const uint8_t *>(buffer), sizeInBytes);
             //   LOGCATI("SendAudioFrame");
                pCloud->SendAudioFrame(audioFrame);
            }
            return oboe::DataCallbackResult::Continue;
        }
    }
}