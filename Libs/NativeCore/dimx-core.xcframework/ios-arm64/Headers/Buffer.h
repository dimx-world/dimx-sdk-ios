#pragma once

#ifndef DIMX_CORE_BUFFER_H
#define DIMX_CORE_BUFFER_H

#include "Common.h"
#include "CerealUtils.hpp"

#include <cstring>
#include <algorithm>

DECLARE_PTR(Buffer)
class Buffer
{
    using size_type = uint64_t; // for 64 / 32-bit cross-compatibility
public:
    Buffer(const Buffer& other)
    : Buffer(other.size(), other.data())
    {}

    Buffer(Buffer&& other) noexcept
    : mData(other.mData)
    , mSize(other.mSize)
    , mCapacity(other.mCapacity)
    {
        // other doesn't own the memory anymore
        other.mSize = 0;
        other.mData = nullptr;
        other.mCapacity = 0;
    }

    Buffer(const size_type size = 0, const void* data = nullptr)
    {
        if (size > 0) {
            reset(size);
            if (data) {
                copyIn(data, size);
            }
        }
    }

    ~Buffer()
    {
        reset();
    }

    Buffer& operator= (const Buffer& other)
    {
        reset(other.size());
        if (other.size() > 0) {
            copyIn(other.data(), other.size());
        }
        return *this;
    }

    Buffer& operator= (Buffer&& other) noexcept
    {
        mSize = other.mSize;
        mData = other.mData;
        mCapacity = other.mCapacity;

        // other doesn't own the memory anymore
        other.mSize = 0;
        other.mData = nullptr;
        other.mCapacity = 0;

        return *this;
    }

    void reset(const size_type size = 0)
    {
        delete[] mData;
        mData = nullptr;
        mCapacity = 0;
        mSize = size;
        if (size > 0) {
            mData = new char[mSize];
            mCapacity = mSize;
            memset(mData, 0, mSize);
        }
    }

    void reserve(const size_type newCapacity) {
        if (newCapacity > mCapacity) {
            char* newData = new char[newCapacity];
            memset(newData, 0, newCapacity);
            if (mSize > 0) {
                std::memcpy(newData, mData, mSize);
            }
            mCapacity = newCapacity;
            delete[] mData;
            mData = newData;
        }        
    }

    void resize(const size_type newSize)
    {
        reserve(newSize);
        mSize = newSize;
    }

    void copyIn(const void* srcBuf, const size_type srcBufSize)
    {
        std::memcpy(mData, srcBuf, std::min(mSize, srcBufSize));
    }

    void assign(const void* srcBuf, const size_type size)
    {
        reset(size);
        copyIn(srcBuf, size);
    }

    void append(const void* srcBuf, const size_type size)
    {
        reserve(mSize + size);
        std::memcpy(mData + mSize, srcBuf, size);
        mSize = mSize + size;
    }

    template <typename T = void*>
    T data()
    {
        static_assert(std::is_pointer_v<T>, "Pointer type is required!");
        return reinterpret_cast<T>(mData);
    }

    template <typename T = void*>
    const T data() const
    {
        static_assert(std::is_pointer_v<T>, "Pointer type is required!");
        return reinterpret_cast<const T>(mData);
    }

    size_type capacity() const
    {
        return mCapacity;
    }

    size_type size() const
    {
        return mSize;
    }

    bool empty() const
    {
        return mSize == 0;
    }

    template <class Archive> void save( Archive & ar ) const
    {
        ARCHIVE(ar, mSize);
        if (mSize > 0) {
            cereal_save_binary(ar, mData, mSize);
        }
    }
      
    template <class Archive> void load( Archive & ar )
    {
        ARCHIVE(ar, mSize);
        reset(mSize);
        if (mSize > 0) {
            cereal_load_binary(ar, mData, mSize);
        }
    }

private:
    // FIXME: the data needs to be aligned not the pointer.
    //        However operator new[] will align it anyway.
    //        ALIGNED probably needs to be removed.
    //        But thoroughly tested afterwards!
    ALIGNED char* mData = nullptr;
    size_type mSize = 0;
    size_type mCapacity = 0;
};

#endif // DIMX_CORE_BUFFER_H