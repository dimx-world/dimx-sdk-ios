#pragma once

#ifndef DIMX_CORE_CIRCULAR_BUFFER_H
#define DIMX_CORE_CIRCULAR_BUFFER_H

#include <vector>

template <typename T>
class CircularBuffer
{
public:
    CircularBuffer(const std::size_t capacity = 0)
    : mData(capacity + 1)
    , mBegin(0)
    , mEnd(0)
    {}

    void reserve(const std::size_t sz)
    {
        mData.resize(sz);
        mBegin = 0;
        mEnd = 0;
    }

    bool empty() const
    {
        return mBegin == mEnd;
    }

    bool full() const
    {
        return mEnd == mData.size() - 1 ? mBegin == 0 : mEnd + 1 == mBegin;
    }

    std::size_t size() const
    {
        return mEnd >= mBegin ? mEnd - mBegin: mData.size() - mBegin + mEnd;
    }

    std::size_t capacity() const
    {
        return mData.size() - 1;
    }

    void push_back(const T& value)
    {
        if (full()) {
            mBegin = advance(mBegin);
        }
        mData[mEnd] = value;
        mEnd = advance(mEnd);
    }

    T pop_front()
    {
        if (empty()) {
            throw std::out_of_range("Nothing to pop");
        }
        const std::size_t pos = mBegin;
        mBegin = advance(mBegin);
        return mData[pos];
    }

    T front() const
    {
        if (empty()) {
            throw std::out_of_range("No front element");
        }
        return mData[mBegin];
    }

    T back() const
    {
        if (empty()) {
            throw std::out_of_range("No back element");
        }
        return mData[(mEnd > 0 ? mEnd : mData.size()) - 1];
    }

    T& operator[] (const std::size_t idx)
    {
        if (idx >= size()) {
            throw std::out_of_range("Indexing beyond buffer size");
        }
        return mData[(mBegin + idx) % mData.size()];
    }

private:
    std::size_t advance(const std::size_t idx) const
    {
        return idx < mData.size() - 1 ? idx + 1 : 0;
    }

private:
    std::vector<T> mData;
    std::size_t mBegin;
    std::size_t mEnd;
};

#endif // DIMX_CORE_CIRCULAR_BUFFER_H