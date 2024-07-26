#pragma once

#include <Common.h>
#include <config/Config.h>

DECL_ENUM(LocationStatus,  Pending,   Active);
DECL_ESTR(LocationStatus, "Pending", "Active");

class Dimension;
DECLARE_PTR(LocationInfo)
class LocationInfo
{
    NO_COPY_MOVE(LocationInfo);

public:
    LocationInfo(Dimension* dim, const Config& config);
    
    ObjectId dimensionId() const;
    ObjectId id() const;
    const std::string& name() const;
    void setName(const std::string& name);

    const std::string& description() const;

    ObjectId icon() const;

    ObjectId refLocation() const;
    void setRefLocation(ObjectId id);

    LocationStatus status() const;
    void setStatus(LocationStatus status);

    bool isAutostart() const;

private:
    ObjectId mDimensionId;
    ObjectId mId;
    std::string mName;
    std::string mDescription;
    ObjectId mIcon;
    ObjectId mRefLocation;
    LocationStatus mStatus = LocationStatus::None;

    bool mAutostart = false;
};
