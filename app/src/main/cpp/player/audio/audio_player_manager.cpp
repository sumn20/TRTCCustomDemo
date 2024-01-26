//
// Created by Sumn on 2024/1/25.
//

#include "audio_player_manager.h"

namespace player {
    namespace audio {
        AudioPlayerManager::AudioPlayerManager() {}

        AudioPlayerManager::~AudioPlayerManager() {
            std::unique_lock<std::mutex> lock(mMutex);
            for (auto &pair: mPlayers) {
                pair.second->releaseAudioPlayerDevice();
                delete pair.second;
            }
            mPlayers.clear();
        }

        basic::Result
        AudioPlayerManager::addPlayer(const std::string &userId, AudioPlayer *player) {
            std::unique_lock<std::mutex> lock(mMutex);
            mPlayers[userId] = player;
            return basic::Result::OK;
        }

        basic::Result AudioPlayerManager::removePlayer(const std::string &userId) {
            std::unique_lock<std::mutex> lock(mMutex);
            auto it = mPlayers.find(userId);
            if (it != mPlayers.end()) {
                delete it->second;
                mPlayers.erase(it);
                return basic::Result::OK;
            }
            return basic::Result::Error;
        }

        AudioPlayer *AudioPlayerManager::getPlayer(const std::string &userId) {
            std::unique_lock<std::mutex> lock(mMutex);
            auto it = mPlayers.find(userId);
            if (it != mPlayers.end()) {
                return it->second;
            }
            return nullptr;
        }

        std::vector<std::string> AudioPlayerManager::getAvailableUsers() {
            std::unique_lock<std::mutex> lock(mMutex);
            std::vector<std::string> userIds;
            for (const auto &pair: mPlayers) {
                userIds.push_back(pair.first);
            }
            return userIds;
        }

        basic::Result AudioPlayerManager::changeAudioApi(oboe::AudioApi audioApi) {
            for (auto &pair: mPlayers) {
                pair.second->changeAudioApi(audioApi);
            }
            return basic::Result::OK;
        }

    }
}