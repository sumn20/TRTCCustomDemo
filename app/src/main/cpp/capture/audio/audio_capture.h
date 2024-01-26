//
// Created by Sumn on 2024/1/23.
//

#ifndef TRTCCUSTOMDEMO_AUDIO_CAPTURE_H
#define TRTCCUSTOMDEMO_AUDIO_CAPTURE_H

#include "oboe.h"
#include "constant.h"
#include "log_util.h"
#include "trtc_cloud.h"
#include "audio_ring_buffer.h"
#include "audio_capture_stream_callback.h"

namespace capture {
    namespace audio {
        class AudioCapture {
        public:
            basic::Result initAudioCaptureDevice(liteav::TRTCCloud *pCloud);

            int releaseAudioCaptureDevice();

            oboe::AudioStream *createAudioStream(oboe::AudioApi api, basic::VolumeType volumeType,
                                                 AudioCaptureStreamCallback *callback);

            void prepareStart();

            void start();

            void stop();

            basic::Result changeStream();

            basic::Result changeAudioApi(oboe::AudioApi audioApi);

            basic::Result setSystemVolumeType(basic::VolumeType volumeType);

            oboe::AudioApi getCurrentAudioApi();

        private:
            liteav::TRTCCloud *pCloud;
            oboe::AudioStream *mStream;
            oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
            RingBuffer *mRingBuffer;
            AudioCaptureStreamCallback *mCallBack;
            basic::VolumeType mVolumeType = basic::VolumeType::MEDIA;
            bool isPrepare = false;
        };
    }
}


#endif //TRTCCUSTOMDEMO_AUDIO_CAPTURE_H
