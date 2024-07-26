#pragma once
#include <Common.h>
#include <Transform.h>

class Location;
namespace edit { class Property; }

DECLARE_PTR(LocationPlaceholder)
class LocationPlaceholder
{
public:
    static constexpr const char* Tag = "Placeholder";

private:
    struct ScaleInfo {
        float min = 0.f;
        float max = 0.f;
        float dir = 1.f;
        float current = 0.f;
    };

public:
    LocationPlaceholder(Location& loc);
    ~LocationPlaceholder();

    void serialize(Config& out);

    void update(const FrameContext& frameContext);

    const Vec3& size() const;
    void setSize(const Vec3& size);

    void setVisible(bool visible);
    void setTransform(const Transform& trans);

private:
    void initRootNode(ObjectPtr root, const Vec3& size);

private:
    Location& mLocation;
    Vec3 mSize{.5f, .25f, .5f};

    std::vector<ObjectPtr> mComponents;
    std::vector<ObjectPtr> mBoxes;

    std::vector<ScaleInfo> mBoxScales;

    ObjectPtr mRootNode;
};
