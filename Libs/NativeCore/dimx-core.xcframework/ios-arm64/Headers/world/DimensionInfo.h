#pragma once
#include <Common.h>
#include <world/Dimension.h>

DECLARE_PTR(DimensionInfo);
class DimensionInfo
{
    NO_COPY_MOVE(DimensionInfo);
public:
    DimensionInfo() = default;
    DimensionInfo(bool isEditable, const Config& config);
    ~DimensionInfo();

    bool isEditable() const;

    ObjectId id() const;
    const std::string& name() const;

    DimensionStatus status() const;
    void setStatus(DimensionStatus st);

    bool isActive() const;

    const ObjectPtr& icon() const { return mIcon; }
    void setIcon(ObjectPtr tex);

private:
    bool mIsEditable{false};
    ObjectId mId;
    std::string mName;
    DimensionStatus mStatus = DimensionStatus::None;
    ObjectPtr mIcon;
    
};
