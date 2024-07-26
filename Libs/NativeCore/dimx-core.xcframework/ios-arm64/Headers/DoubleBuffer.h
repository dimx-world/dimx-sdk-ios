#pragma once

#ifndef DIMX_CORE_DOUBLE_BUFFER_H
#define DIMX_CORE_DOUBLE_BUFFER_H

#include <Common.h>
#include <array>

template <typename T>
class DoubleBuffer
{
    NO_COPY_MOVE(DoubleBuffer)

public:
    DoubleBuffer() = default;

    void write(T item) {
        mData[0].emplace_back(std::move(item));
    }

    std::vector<T>& read() {
        return mData[1];
    }

    void swap() {
        mData[1].clear();
        mData[0].swap(mData[1]);
    }

    void clear() {
        mData[0].clear();
        mData[1].clear();
    }

private:
    // write to mEvents[0], read from mEvents[1]
    // keep two buffers to prevent frequent re-allocations
    std::array<std::vector<T>, 2> mData;
};

#endif // DIMX_CORE_DOUBLE_BUFFER_H