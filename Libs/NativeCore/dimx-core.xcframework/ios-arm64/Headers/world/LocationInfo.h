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

    ObjectId icon() const;

    LocationStatus status() const;
    void setStatus(LocationStatus status);

private:
    ObjectId mDimensionId;
    ObjectId mId;
    std::string mName;
    ObjectId mIcon;
    LocationStatus mStatus = LocationStatus::None;
};
