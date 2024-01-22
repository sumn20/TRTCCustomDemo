//
// Created by Sumn on 2024/1/18.
//

#include "audio_player.h"

namespace player {
    namespace audio {
        basic::Result AudioPlayer::initAudioPlayerDevice() {
            size_t bufferSize = 48000 * 2 * sizeof(int16_t);
            mRingBuffer = new RingBuffer(
                    bufferSize);
            mCallBack = new MyAudioStreamCallback(mRingBuffer);
            mStream = createAudioStream(mAudioApi, mCallBack);
            if (mStream != nullptr) {
                return basic::Result::OK;
            } else {
                LOGCATE("initAudioPlayerDevice error");
                return basic::Result::ErrorAudioInit;
            }
        }


        oboe::AudioStream *AudioPlayer::createAudioStream(oboe::AudioApi audioApi,
                                                          oboe::AudioStreamCallback *callback) {
            basic::AudioConfig audioConfig;
            oboe::AudioStreamBuilder builder;
            oboe::AudioStream *audioStream = nullptr;
            builder.setPerformanceMode(oboe::PerformanceMode::LowLatency)
                    ->setSharingMode(oboe::SharingMode::Exclusive)
                    ->setAudioApi(audioApi)
                    ->setFormat(AudioFormat::I16)
                    ->setSampleRate(audioConfig.sampleRate)
                    ->setChannelCount(audioConfig.channelCount)
                    ->setCallback(callback);
            oboe::Result result = builder.openStream(&audioStream);
            if (result != oboe::Result::OK) {
                LOGCATE("createAudioStream error:%s", oboe::convertToText(result));
            }
            return audioStream;
        }

        basic::Result AudioPlayer::start(const uint8_t *data, size_t dataSize) {
            if (mStream != nullptr) {
                mRingBuffer->write(data, dataSize);
                if (mRingBuffer->getSize() >= 48000 * 2 * sizeof(int16_t) &&
                    mStream->getState() != oboe::StreamState::Started) {
                    oboe::Result result = mStream->requestStart();
                    if (result != oboe::Result::OK) {
                        LOGCATE("start error:%s", oboe::convertToText(result));
                        return basic::Result::ErrorAudioPlayer;
                    }
                }
            }
            return basic::Result::OK;
        }

        oboe::AudioApi AudioPlayer::getCurrentAudioApi() {
            if (mStream != nullptr) {
                return mStream->getAudioApi();
            } else {
                return oboe::AudioApi::Unspecified;
            }
        }

        basic::Result AudioPlayer::changeAudioApi(oboe::AudioApi audioApi) {
            if (audioApi != mAudioApi) {
                mStream->requestStop();
                mStream->close();
                mAudioApi = audioApi;
                oboe::AudioStream *tmpStream = createAudioStream(mAudioApi, mCallBack);
                if (tmpStream != nullptr) {
                    mStream = tmpStream;
                    return basic::Result::OK;
                } else {
                    LOGCATE("changeAudioApi error");
                    return basic::Result::ErrorAudioChange;
                }
            }
            return basic::Result::OK;
        }


        int AudioPlayer::releaseAudioPlayerDevice() {
            if (mStream != nullptr) {
                mStream->requestStop();
                mStream->close();
                mStream = nullptr;
                delete mCallBack;
                mCallBack = nullptr;
            }
            return 0;
        }
    }
}