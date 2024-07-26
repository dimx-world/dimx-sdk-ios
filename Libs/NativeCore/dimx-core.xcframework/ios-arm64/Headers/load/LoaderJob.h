#pragma once
#include <Common.h>

class LoaderJob
{
    NO_COPY_MOVE(LoaderJob);

public:
    LoaderJob() = default;
    virtual ~LoaderJob() = default;
    ObjectId id() const { return mId; }

    virtual void run() = 0;

private:
    ObjectId mId{ObjectId::random()};
};
