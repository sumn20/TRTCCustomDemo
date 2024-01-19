//
// Created by Sumn on 2024/1/18.
//

#include "audio_ring_buffer.h"
namespace player {
namespace audio {


    RingBuffer::RingBuffer(size_t capacity) : mData(capacity), mWritePos(0), mReadPos(0), mSize(0) {}

    size_t RingBuffer::write(const uint8_t *data, size_t numBytes) {
        size_t bytesWritten = 0;

        while (bytesWritten < numBytes) {
            size_t bytesToWrite = std::min(numBytes - bytesWritten, mData.size() - mWritePos);
            std::copy(data + bytesWritten, data + bytesWritten + bytesToWrite, mData.begin() + mWritePos);

            bytesWritten += bytesToWrite;
            mWritePos = (mWritePos + bytesToWrite) % mData.size();
        }

        mSize = std::min(mSize + bytesWritten, mData.size());
        return bytesWritten;
    }

    size_t RingBuffer::read(uint8_t *data, size_t numBytes) const {
        size_t bytesRead = 0;

        while (bytesRead < numBytes && mSize > 0) {
            size_t bytesToRead = std::min(numBytes - bytesRead, mData.size() - mReadPos);
            std::copy(mData.begin() + mReadPos, mData.begin() + mReadPos + bytesToRead, data + bytesRead);

            bytesRead += bytesToRead;
            mReadPos = (mReadPos + bytesToRead) % mData.size();
            mSize -= bytesToRead;
        }

        return bytesRead;
    }

    size_t RingBuffer::getSize() {
        return mSize;
    }
}}