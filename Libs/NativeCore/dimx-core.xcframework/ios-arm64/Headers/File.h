#pragma once

#ifndef DIMX_CORE_FILE_H
#define DIMX_CORE_FILE_H

#include <ecs/Component.h>
#include "Buffer.h"

class File: public Component
{
    DECLARE_COMPONENT(File)

public:
    static ConfigPtr makeConfig(const std::string& name);
    static bool readyToCreate(const Config& config);

    File(Object* entity, const Config& config);
    void serialize(Config& out) override;

    const BufferPtr& buffer() const;
    const std::string& fileId() const;

private:
    BufferPtr mBuffer;
    std::string mFileId;
    bool mSkipDataSave = false;
};

#endif // DIMX_CORE_FILE_H