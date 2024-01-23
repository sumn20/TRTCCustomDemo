//
// Created by Sumn on 2024/1/23.
//

#include "audio_capture.h"


namespace capture {
    namespace audio {
        basic::Result AudioCapture::initAudioCaptureDevice(liteav::TRTCCloud *trtcCloud) {
            size_t bufferSize = 48000 * 2 * sizeof(int16_t);
            mRingBuffer = new RingBuffer(
                    bufferSize);
            pCloud = trtcCloud;
            mStream = createAudioStream(mAudioApi);
            if (mStream != nullptr) {
                return basic::Result::OK;
            } else {
                LOGCATE("initAudioCaptureDevice error");
                return basic::Result::ErrorAudioInit;
            }
        }

        void AudioCapture::start() {
            if (mStream) {
                LOGCATI("AudioCapture start");
                mStream->requestStart();
            }
        }

        void AudioCapture::stop() {
            if (mStream) {
                LOGCATI("AudioCapture stop");
                mStream->requestStop();
            }
        }

        basic::Result AudioCapture::changeAudioApi(oboe::AudioApi audioApi) {
            if (audioApi != mAudioApi) {
                mStream->requestStop();
                mStream->close();
                mAudioApi = audioApi;
                oboe::AudioStream *tmpStream = createAudioStream(mAudioApi);
                if (tmpStream != nullptr) {
                    mStream = tmpStream;
                    start();
                    return basic::Result::OK;
                } else {
                    LOGCATE("changeAudioApi error");
                    return basic::Result::ErrorAudioChange;
                }
            }
            return basic::Result::OK;
        }

        oboe::DataCallbackResult
        AudioCapture::onAudioReady(oboe::AudioStream *oboeStream, void *audioData,
                                   int32_t numFrames) {
            mRingBuffer->write(static_cast<uint8_t *>(audioData),
                               numFrames * oboeStream->getBytesPerFrame());
            int32_t framesFor20ms = static_cast<int32_t>(oboeStream->getSampleRate() * 0.02);
            int32_t sizeInBytes = framesFor20ms * oboeStream->getBytesPerFrame();
            if (mRingBuffer->getSize() >= sizeInBytes) {
                uint8_t buffer[sizeInBytes];
                mRingBuffer->read(buffer, sizeInBytes);
                basic::AudioConfig audioConfig;
                liteav::AudioFrame audioFrame;
                audioFrame.channels = audioConfig.channelCount;
                audioFrame.sample_rate = audioConfig.sampleRate;
                audioFrame.SetData(static_cast<const uint8_t *>(buffer), sizeInBytes);
                LOGCATI("SendAudioFrame");
                pCloud->SendAudioFrame(audioFrame);
            }
            return oboe::DataCallbackResult::Continue;
        }


        int AudioCapture::releaseAudioCaptureDevice() {
            if (mStream != nullptr) {
                isPrepare= false;
                mStream->requestStop();
                mStream->close();
                mStream = nullptr;
            }
            if (pCloud != nullptr) {
                pCloud->DestroyLocalAudioChannel();
            }
            return 0;
        }

        oboe::AudioStream *AudioCapture::createAudioStream(oboe::AudioApi api) {
            basic::AudioConfig audioConfig;
            oboe::AudioStreamBuilder builder;
            oboe::AudioStream *audioStream = nullptr;
            builder.setDirection(oboe::Direction::Input)
                    ->setFormat(oboe::AudioFormat::I16)
                    ->setChannelCount(audioConfig.channelCount)
                    ->setSampleRate(audioConfig.sampleRate)
                    ->setAudioApi(api)
                    ->setCallback(this);

            oboe::Result result = builder.openStream(&audioStream);
            if (result != oboe::Result::OK) {
                LOGCATE("initAudioCaptureDevice error:%s", oboe::convertToText(result));
            }
            return audioStream;
        }

        void AudioCapture::prepareStart() {
            if (isPrepare) {
                start();
            } else {
                isPrepare = true;
                LOGCATI("AudioCapture prepareStart");
                basic::AudioConfig audioConfig;
                liteav::AudioEncodeParams encodeParams;
                encodeParams.sample_rate = audioConfig.sampleRate;
                encodeParams.channels = audioConfig.channelCount;
                pCloud->CreateLocalAudioChannel(encodeParams);
            }

        }


    }
}