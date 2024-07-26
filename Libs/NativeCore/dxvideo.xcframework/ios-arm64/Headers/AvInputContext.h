#pragma once

struct AVIOContext;
class AvInputContext
{
public:
    virtual ~AvInputContext() = default;
    virtual AVIOContext* ioContext() = 0;
    virtual void stop() {};
    virtual int64_t getLoadedSize() { return 0; };
};
