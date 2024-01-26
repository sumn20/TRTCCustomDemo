//
// Created by Sumn on 2024/1/18.
//

#include "audio_player.h"

namespace player {
    namespace audio {
        basic::Result AudioPlayer::initAudioPlayerDevice() {
            LOGCATE("initAudioPlayerDevice");
            size_t bufferSize =
                    mAudioConfig.sampleRate * mAudioConfig.channelCount * sizeof(int16_t);
            mRingBuffer = new RingBuffer(
                    bufferSize);
            mCallBack = new AudioPlayerStreamCallback(mRingBuffer);
            mStream = createAudioStream(mAudioApi, mVolumeType, mCallBack);
            if (mStream != nullptr) {
                return basic::Result::OK;
            } else {
                LOGCATE("initAudioPlayerDevice error");
                return basic::Result::ErrorAudioInit;
            }
        }


        oboe::AudioStream *
        AudioPlayer::createAudioStream(oboe::AudioApi audioApi, basic::VolumeType volumeType,
                                       oboe::AudioStreamCallback *callback) {

            oboe::AudioStreamBuilder builder;
            oboe::AudioStream *audioStream = nullptr;
            builder.setPerformanceMode(oboe::PerformanceMode::LowLatency)
                    ->setSharingMode(oboe::SharingMode::Exclusive)
                    ->setAudioApi(audioApi)
                    ->setFormat(AudioFormat::I16)
                    ->setSampleRate(mAudioConfig.sampleRate)
                    ->setChannelCount(mAudioConfig.channelCount)
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
                LOGCATE("createAudioStream error:%s", oboe::convertToText(result));
            }
            return audioStream;
        }

        basic::Result
        AudioPlayer::start(const uint8_t *data, size_t dataSize, int sample_rate,int channels) {
            if (!isInit) {
                isInit = true;
                mAudioConfig.sampleRate=sample_rate;
                mAudioConfig.channelCount=channels;
                initAudioPlayerDevice();
            }
            if (mStream != nullptr) {
                mRingBuffer->write(data, dataSize);
                if (mStream->getState() != oboe::StreamState::Started) {
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

        basic::Result AudioPlayer::changeStream() {
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

        basic::Result AudioPlayer::changeAudioApi(oboe::AudioApi audioApi) {
            LOGCATI("AudioPlayer changeAudioApi");
            if (audioApi != mAudioApi) {
                mAudioApi = audioApi;
                return changeStream();
            }
            return basic::Result::OK;
        }

        basic::Result AudioPlayer::setSystemVolumeType(basic::VolumeType volumeType) {
            LOGCATI("AudioPlayer setSystemVolumeType");
            if (volumeType != mVolumeType) {
                mVolumeType = volumeType;
                return changeStream();
            }
            return basic::Result::OK;
        }


        int AudioPlayer::releaseAudioPlayerDevice() {
            if (mStream != nullptr) {
                isInit= false;
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