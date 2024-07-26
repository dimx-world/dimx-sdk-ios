#pragma once
#ifndef DIMX_CORE_BUFFER_STREAM_H
#define DIMX_CORE_BUFFER_STREAM_H

#include <Buffer.h>

class OBufferStream
{
public:
    OBufferStream(Buffer& buff)
    : mBuffer(buff)
    {}

    template <typename T>
    OBufferStream& operator<<(const T& val)
    {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Integral required.");
        write(sizeof(val), &val);
        return *this;
    }

    void write(size_t count, const void* src)
    {
        if (mBuffer.size() < mCurrentPos + count) {
            mBuffer.resize(2 * (mCurrentPos + count));
        }

        std::memcpy(mBuffer.data<char*>() + mCurrentPos, src, count);

        mCurrentPos += count;
    }

    size_t bytesWritten() const
    {
        return mCurrentPos;
    }

private:
    Buffer& mBuffer;
    size_t mCurrentPos{0};
};

template <> OBufferStream& OBufferStream::operator<<(const std::string& val)
{
    *this << static_cast<uint32_t>(val.size());
    if (!val.empty()) {
        write(val.size(), val.c_str());
    }
    return *this;
}

template <> OBufferStream& OBufferStream::operator<<(const Buffer& val)
{
    *this << static_cast<uint32_t>(val.size());
    if (!val.empty()) {
        write(val.size(), val.data());
    }
    return *this;
}
////////////////////////////////////////////////////////////////////////////////

class IBufferStream
{
public:
    IBufferStream(const char* buff, size_t size)
    : mBuffer(buff)
    , mBufferSize(size)
    {
        ASSERT(mBuffer && mBufferSize > 0, "Invalid buffer");
    }

    template <typename T>
    IBufferStream& operator>>(T& val)
    {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Integral required.");
        read(sizeof(val), &val);
        return *this;
    }

    void read(size_t count, void* src)
    {
        ASSERT(mCurrentPos + count <= mBufferSize, "Reading beyond buffer!");
        std::memcpy(src, mBuffer + mCurrentPos, count);
        mCurrentPos += count;
    }

    size_t bytesRead() const
    {
        return mCurrentPos;
    }

private:
    const char* mBuffer{nullptr};
    size_t mBufferSize{0};
    size_t mCurrentPos{0};
};

template <> IBufferStream& IBufferStream::operator>>(std::string& val)
{
    uint32_t size{0};
    *this >> size;

    ASSERT(mCurrentPos + size <= mBufferSize, "Reading string beyond buffer!");

    if (size == 0) {
        val.clear();
    } else {
        val.assign(mBuffer + mCurrentPos, size);
    }

    mCurrentPos += size;
    return *this;
}

template <> IBufferStream& IBufferStream::operator>>(Buffer& val)
{
    uint32_t size{0};
    *this >> size;

    ASSERT(mCurrentPos + size <= mBufferSize, "Reading string beyond buffer!");

    if (size == 0) {
        val.reset();
    } else {
        val.assign(mBuffer + mCurrentPos, size);
    }

    mCurrentPos += size;
    return *this;
}

#endif // DIMX_CORE_BUFFER_STREAM_H