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
            mCallBack = new AudioCaptureStreamCallback(mRingBuffer, pCloud);
            mStream = createAudioStream(mAudioApi, mVolumeType, mCallBack);
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

        basic::Result AudioCapture::changeStream() {
            if (mStream) {
                mStream->requestStop();
                mStream->close();
                oboe::AudioStream *tmpStream = createAudioStream(mAudioApi, mVolumeType, mCallBack);
                if (tmpStream != nullptr) {
                    mStream = tmpStream;
                    return basic::Result::OK;
                } else {
                    LOGCATE("changeAudioApi error");
                    return basic::Result::ErrorAudioChange;
                }
            } else {
                return basic::Result::ErrorNull;
            }

        }

        basic::Result AudioCapture::changeAudioApi(oboe::AudioApi audioApi) {
            LOGCATI("AudioCapture changeAudioApi");
            if (audioApi != mAudioApi) {
                mAudioApi = audioApi;
                return changeStream();
            }
            return basic::Result::OK;
        }

        basic::Result AudioCapture::setSystemVolumeType(basic::VolumeType volumeType) {
            LOGCATI("AudioCapture setSystemVolumeType");
            if (volumeType != mVolumeType) {
                mVolumeType = volumeType;
                changeStream();
            }
            return basic::Result::OK;
        }

        int AudioCapture::releaseAudioCaptureDevice() {
            if (mStream != nullptr) {
                isPrepare = false;
                mStream->requestStop();
                mStream->close();
                mStream = nullptr;
                delete mCallBack;
                mCallBack = nullptr;
            }
            if (pCloud != nullptr) {
                pCloud->DestroyLocalAudioChannel();
            }
            return 0;
        }

        oboe::AudioStream *
        AudioCapture::createAudioStream(oboe::AudioApi api, basic::VolumeType volumeType,
                                        AudioCaptureStreamCallback *callback) {
            basic::AudioConfig audioConfig;
            oboe::AudioStreamBuilder builder;
            oboe::AudioStream *audioStream = nullptr;
            builder.setDirection(oboe::Direction::Input)
                    ->setFormat(oboe::AudioFormat::I16)
                    ->setChannelCount(audioConfig.channelCount)
                    ->setSampleRate(audioConfig.sampleRate)
                    ->setAudioApi(api)
                    ->setCallback(callback);
            if (volumeType == basic::VolumeType::MEDIA) {
                builder.setUsage(oboe::Usage::Media);
                builder.setContentType(oboe::ContentType::Music);
            } else {
                builder.setUsage(oboe::Usage::VoiceCommunication);
                builder.setContentType(oboe::ContentType::Speech);
            }
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

        oboe::AudioApi AudioCapture::getCurrentAudioApi() {
            if (mStream != nullptr) {
                return mStream->getAudioApi();
            } else {
                return oboe::AudioApi::Unspecified;
            }
        }


    }
}