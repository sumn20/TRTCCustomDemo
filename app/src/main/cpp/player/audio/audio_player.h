//
// Created by Sumn on 2024/1/18.
//
#include "Oboe.h"
#include "audio_ring_buffer.h"
#include "audio_player_stream_callback.h"
#include "log_util.h"
#include "constant.h"

#ifndef TRTCCUSTOMDEMO_AUDIO_PLAYER_H
#define TRTCCUSTOMDEMO_AUDIO_PLAYER_H
using namespace oboe;
namespace player {
    namespace audio {
        class AudioPlayer {

        public:


            oboe::AudioStream *
            createAudioStream(oboe::AudioApi audioApi, basic::VolumeType volumeType,
                              oboe::AudioStreamCallback *callback);

            basic::Result start(const uint8_t *data, size_t dataSize,int sample_rate,int channels);

            basic::Result changeAudioApi(oboe::AudioApi audioApi);

            basic::Result changeStream();

            oboe::AudioApi getCurrentAudioApi();

            int releaseAudioPlayerDevice();

            basic::Result setSystemVolumeType(basic::VolumeType volumeType);

        private:
            basic::Result initAudioPlayerDevice();
            oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
            basic::VolumeType mVolumeType = basic::VolumeType::MEDIA;
            oboe::AudioStream *mStream;
            AudioPlayerStreamCallback *mCallBack;
            RingBuffer *mRingBuffer;
            bool isInit = false;
            basic::AudioConfig mAudioConfig;

        };
    }
}

#endif //TRTCCUSTOMDEMO_AUDIO_PLAYER_H
