//
// Created by Sumn on 2022/7/5.
//


#include "trtc_core.h"


TRTCCloudCore *TRTCCloudCore::m_instance = nullptr;

TRTCCloudCore *TRTCCloudCore::GetInstance() {
    if (m_instance == nullptr) {
        m_instance = new TRTCCloudCore();
    }
    return m_instance;
}

void TRTCCloudCore::Destroy() {
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

TRTCCloudCore::TRTCCloudCore() {
    LOGCATI("TRTCCloudCore()");
    liteav::InitParams init_params;
    TRTCCloud::Initialize(init_params);
    pCloud = TRTCCloud::Create(this);
    pAudioCapture = new capture::audio::AudioCapture();
    pAudioCapture->initAudioCaptureDevice(pCloud);
    mKeepRunning = true;
    std::thread playbackThread(&TRTCCloudCore::audioPlaybackThread, this);
    playbackThread.detach();
    pAudioPlayerManager = new player::audio::AudioPlayerManager();
}

TRTCCloudCore::~TRTCCloudCore() {
    LOGCATI("~TRTCCloudCore()");
    mKeepRunning = false;
    if (pAudioCapture != nullptr) {
        pAudioCapture->releaseAudioCaptureDevice();
        delete pAudioCapture;
    }
    TRTCCloud::Destroy(pCloud);
    pCloud = nullptr;
    delete pAudioPlayerManager;
    pAudioPlayerManager = nullptr;

}

TRTCCloud *TRTCCloudCore::getTRTCCloud() {
    return pCloud;
}

void TRTCCloudCore::OnError(Error error) {
    LOGCATE("onError errorCode[%d]", error);
}

void TRTCCloudCore::OnConnectionStateChanged(ConnectionState state) {
    LOGCATI("OnConnectionStateChanged old_state[%d]", state);
}

void TRTCCloudCore::OnEnterRoom() {
    LOGCATI("onEnterRoom");
}

void TRTCCloudCore::OnExitRoom() {
    LOGCATI("OnExitRoom");
}

void TRTCCloudCore::OnLocalAudioChannelCreated() {
    LOGCATI("OnLocalAudioChannelCreated");
    if (pAudioCapture != nullptr) {
        pAudioCapture->start();
    }
}

void TRTCCloudCore::OnLocalAudioChannelDestroyed() {
    LOGCATI("OnLocalAudioChannelDestroyed");
}

void TRTCCloudCore::OnLocalVideoChannelCreated(StreamType type) {
    LOGCATI("OnLocalVideoChannelCreated StreamType[%d]", type);
}

void TRTCCloudCore::OnLocalVideoChannelDestroyed(StreamType type) {
    LOGCATI("OnLocalVideoChannelDestroyed StreamType[%d]", type);
}

void TRTCCloudCore::OnRequestChangeVideoEncodeBitrate(StreamType type, int bitrate_bps) {
    LOGCATI("OnRequestChangeVideoEncodeBitrate StreamType[%d] bitrate_bps[%d]", type, bitrate_bps);
}

void TRTCCloudCore::OnRemoteUserEnterRoom(const UserInfo &info) {
    LOGCATI("OnRemoteUserEnterRoom UserInfo[%s]", info.user_id);
}

void TRTCCloudCore::OnRemoteUserExitRoom(const UserInfo &info) {
    LOGCATI("OnRemoteUserExitRoom UserInfo[%s]", info.user_id);
}

void TRTCCloudCore::OnRemoteAudioAvailable(const char *user_id, bool available) {
    LOGCATI("OnRemoteAudioAvailable user_id[%s] available[%d]", user_id, available);
    std::string userId(user_id);
    if (available) {
        auto *player = new player::audio::AudioPlayer();
        pAudioPlayerManager->addPlayer(userId, player);
    } else {
        pAudioPlayerManager->removePlayer(userId);
    }

}

void TRTCCloudCore::OnRemoteVideoAvailable(const char *user_id, bool available, StreamType type) {
    LOGCATI("OnRemoteVideoAvailable user_id[%s] available[%d] StreamType[%d]", user_id, available,
            type);
}

void TRTCCloudCore::OnRemoteVideoReceived(const char *user_id, StreamType type,
                                          const VideoFrame &frame) {


}

void TRTCCloudCore::OnRemoteAudioReceived(const char *user_id, const AudioFrame &frame) {
    LOGCATI("OnRemoteAudioReceived");
}

void TRTCCloudCore::OnRemoteMixedAudioReceived(const AudioFrame &frame) {
    LOGCATI("OnRemoteMixedAudioReceived");

}

int TRTCCloudCore::getCurrentAudioApi() {
    LOGCATD("getCurrentAudioApi");
    if (pAudioCapture != nullptr) {
        int currentApi = static_cast<int>(pAudioCapture->getCurrentAudioApi());
        return currentApi;
    } else {
        return static_cast<int>(AudioApi::Unspecified);
    }

}

void TRTCCloudCore::changeAudioApi(oboe::AudioApi audioApi) {
    LOGCATD("changeAudioApi");
    if (pAudioPlayerManager != nullptr) {
        pAudioPlayerManager->changeAudioApi(audioApi);
    }
    if (pAudioCapture != nullptr) {
        pAudioCapture->changeAudioApi(audioApi);
    }
}

void TRTCCloudCore::startCapture() {
    if (pAudioCapture == nullptr) {
        pAudioCapture = new capture::audio::AudioCapture();
    }
    pAudioCapture->prepareStart();
}

void TRTCCloudCore::stopCapture() {
    if (pAudioCapture != nullptr) {
        pAudioCapture->stop();
    }

}

void TRTCCloudCore::audioPlaybackThread() {
    while (mKeepRunning) {
        std::vector<std::string> userIds = pAudioPlayerManager->getAvailableUsers();
        for (const std::string &userId: userIds) {
            AudioFrame frame;
            int bytesRead = pCloud->GetAudioFrame(userId.c_str(), &frame);
            if (bytesRead > 0) {
                player::audio::AudioPlayer *player = pAudioPlayerManager->getPlayer(userId);
                if (player) {
                    LOGCATI("sample_rate:%d,channels:%d", frame.sample_rate, frame.channels);
                    player->start(frame.data(), frame.size(), frame.sample_rate, frame.channels);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

