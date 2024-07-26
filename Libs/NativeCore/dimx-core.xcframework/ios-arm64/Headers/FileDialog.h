#pragma once

#ifndef DIMX_CORE_FILE_DIALOG_H
#define DIMX_CORE_FILE_DIALOG_H

#include "CrossObject.h"
#include <Common.h>
#include <Buffer.h>

struct OpenFileInfo
{
    std::string name;
    BufferPtr data;
};

DECL_ENUM(FileFilter,  All,   Texture,   Binary)
DECL_ESTR(FileFilter, "All", "Texture", "Binary")

DECL_ENUM(FileDialogMode,  Single,   Multiple)
DECL_ESTR(FileDialogMode, "Single", "Multiple")

class FileDialog: public CrossObject
{
public:
    FileDialog() = default;
    virtual ~FileDialog() = default;
    virtual void openFiles(FileFilter filter,
                          FileDialogMode mode,
                          std::function<void(std::vector<OpenFileInfo>)> callback) {}
};

#endif // DIMX_CORE_FILE_DIALOG_H