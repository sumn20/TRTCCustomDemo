//
// Created by Sumn on 2024/1/18.
//

#ifndef TRTCCUSTOMDEMO_AUDIO_RING_BUFFER_H
#define TRTCCUSTOMDEMO_AUDIO_RING_BUFFER_H

#include <vector>
#include <cstdint>
#include <algorithm>

namespace player {
    namespace audio {
        class RingBuffer {
        public:
            RingBuffer(size_t capacity);

            size_t write(const uint8_t *data, size_t numBytes);
            size_t read(uint8_t *data, size_t numBytes) const;

            size_t getSize();

        private:
            mutable std::vector<uint8_t> mData;
            mutable size_t mWritePos;
            mutable size_t mReadPos;
            mutable size_t mSize;
        };
    }
}


#endif //TRTCCUSTOMDEMO_AUDIO_RING_BUFFER_H
