#pragma once
#include <Common.h>
#include <Buffer.h>
#include <AvInputContext.h>

class RemoteIStream;
class AvStreamInputContext: public AvInputContext
{
    static constexpr size_t BUFFER_SIZE = 4 * 1024;
    NO_COPY_MOVE(AvStreamInputContext);
public:
    AvStreamInputContext(ObjectId dim, ObjectId loc, const std::string& file);
    ~AvStreamInputContext() override;
    AVIOContext* ioContext() override { return mIoContext; }

    void stop() override;

    int read(uint8_t *buf, int bufSize);
    int64_t seek(int64_t offset, int whence);

    int64_t getLoadedSize() override;

private:
    std::shared_ptr<RemoteIStream> mWebStream;
    AVIOContext* mIoContext{nullptr};
    std::atomic_bool mKeepRunning{true};
};
