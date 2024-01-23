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
namespace capture{
    namespace audio{
        class AudioCapture  : public oboe::AudioStreamCallback {
        public:
            basic::Result initAudioCaptureDevice( liteav::TRTCCloud *pCloud);
            int releaseAudioCaptureDevice();
            oboe::AudioStream * createAudioStream(oboe::AudioApi api);
            void prepareStart();
            void start();
            void stop();
            basic::Result changeAudioApi(oboe::AudioApi audioApi);

            oboe::DataCallbackResult
            onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

        private:
            liteav::TRTCCloud *pCloud ;
            oboe::AudioStream *mStream;
            oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
            RingBuffer *mRingBuffer;
            bool isPrepare= false;
        };
    }
}



#endif //TRTCCUSTOMDEMO_AUDIO_CAPTURE_H
