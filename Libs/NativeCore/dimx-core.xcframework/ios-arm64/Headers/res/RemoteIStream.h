#pragma once
#include <Common.h>
#include <Buffer.h>
#include <mutex>

DECL_ENUM(SeekWhence,  Begin,   Current,   End)
DECL_ESTR(SeekWhence, "Begin", "Current", "End")

class RemoteIStream: public std::enable_shared_from_this<RemoteIStream>
{
    using SysTime = std::chrono::time_point<std::chrono::system_clock>;
    static SysTime systemNow() { return std::chrono::system_clock::now(); }

private:
    static constexpr size_t CHUNK_SIZE = 256 * 1024;
    static constexpr size_t PRELOAD_SIZE = 5 * 1024 * 1024;

    // Emulate slow connection:
    //static constexpr size_t CHUNK_SIZE = 20 * 1024;
    //static constexpr size_t PRELOAD_SIZE = 200 * 1024;

public:
    RemoteIStream(ObjectId dim, ObjectId loc, const std::string& file);
    ~RemoteIStream();

    void initialize();

    int64_t getTotalSize();
    int64_t getLoadedSize();

    size_t read(void* buf, size_t bufSize);
    int64_t seek(int64_t offset, SeekWhence whence, bool force = false);

    bool eof() { return mEOF; }

private:
    void tryFetchNextChunk();
    void onRemoteResponse(const std::string& error, ConfigPtr response);

private:
    ObjectId mDimensionId;
    ObjectId mLocationId;
    std::string mFilename;

    size_t mTotalSize{0};
    size_t mLoadedSize{0};
    std::vector<BufferPtr> mChunks;
    bool mChunkRequestInProgress{false};
    SysTime mLastChunkRequestTime;
    bool mRemoteError{false};

    size_t mReadPos{0};
    bool mEOF{false};

    std::mutex mMutex;
};
