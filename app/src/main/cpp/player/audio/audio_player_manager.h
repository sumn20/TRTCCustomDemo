//
// Created by Sumn on 2024/1/25.
//

#ifndef TRTCCUSTOMDEMO_AUDIO_PLAYER_MANAGER_H
#define TRTCCUSTOMDEMO_AUDIO_PLAYER_MANAGER_H

#include <map>
#include <string>
#include <mutex>
#include "constant.h"
#include "audio_player.h"

namespace player {
    namespace audio {
        class AudioPlayerManager {
        public:
            AudioPlayerManager();

            ~AudioPlayerManager();

            basic::Result addPlayer(const std::string &userId, AudioPlayer *player);

            basic::Result removePlayer(const std::string &userId);

            AudioPlayer *getPlayer(const std::string &userId);

            std::vector<std::string> getAvailableUsers();

            basic::Result changeAudioApi(oboe::AudioApi audioApi);

        private:
            std::map<std::string, AudioPlayer *> mPlayers;
            std::mutex mMutex;
        };
    }
}


#endif //TRTCCUSTOMDEMO_AUDIO_PLAYER_MANAGER_H
