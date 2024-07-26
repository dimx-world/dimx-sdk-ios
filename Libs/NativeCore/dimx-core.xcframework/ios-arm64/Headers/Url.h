#pragma once

#ifndef DIMX_CORE_URL_H
#define DIMX_CORE_URL_H

#include <Common.h>

class Url
{
public:
    Url() = default;
    Url(const Url& other) = default;
    Url& operator =(const Url& other) = default;
    Url(const std::string& str);

    bool valid() const { return !mStr.empty(); }
    const std::string& str() const { return mStr; }
    const std::string& host() const { return mHost; }
    ObjectId dimension() const { return mDimension; }
    ObjectId location() const { return mLocation; }
    bool edit() const { return mEdit; }
    bool scan() const { return mScan; }
    bool live() const { return mLive; }
    bool force() const { return mForce; }
    bool place() const { return mPlace; }
    const std::string& custom() const { return mCustom; }

private:
    bool parseStr(const std::string& str);

private:
    std::string mStr;
    std::string mHost;
    ObjectId mDimension;
    ObjectId mLocation;
    bool mEdit{false};
    bool mScan{false};
    bool mLive{false};
    bool mForce{false};
    bool mPlace{false};
    std::string mCustom;
};

#endif // DIMX_CORE_URL_H