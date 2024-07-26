#pragma once

#ifndef DIMX_CORE_FILE_SYSTEM_H
#define DIMX_CORE_FILE_SYSTEM_H

#include <CrossObject.h>
#include <Buffer.h>
#include <DateTime.h>

class FileSystem: public CrossObject
{
public:
    FileSystem();
    virtual ~FileSystem();
    static FileSystem& inst();

    void initialize(const std::string& appDataPath,
                    const std::string& localStoragePath,
                    const std::string& cachePath,
                    const std::string& extMediaPath);

    const std::string& appDataPath() const { return mAppDataPath; }
    const std::string& localStoragePath() const { return mLocalStoragePath; }
    const std::string& cachePath() const { return mCachePath; }
    const std::string& extMediaPath() const { return mExtMediaPath; }

    virtual bool fileExists(const std::string& path);
    virtual bool dirExists(const std::string& path);
    virtual BufferPtr loadFile(const std::string& path);
    virtual void saveFile(const std::string& path, const Buffer& data);
    virtual void setFileMTime(const std::string& path, const DateTime& mtime);
    virtual bool getFileMTime(const std::string& path, DateTime& outTime);
    virtual bool remove(const std::string& path);

    void makeDir(const std::string& path);
    virtual std::vector<std::string> listFiles(const std::string& path) const;

protected:
    std::string makeAbsPath(const std::string& path) const;

private:
    void doMakeDir(const std::string& path);

private:
    std::string mAppDataPath;
    std::string mLocalStoragePath;
    std::string mCachePath;
    std::string mExtMediaPath;

};

inline FileSystem& g_fileSystem() { return FileSystem::inst(); }

#endif // DIMX_CORE_FILE_SYSTEM_H