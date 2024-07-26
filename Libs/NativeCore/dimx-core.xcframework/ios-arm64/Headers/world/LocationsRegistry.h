#pragma once

#include <Common.h>
#include <config/Config.h>
#include <world/LocationInfo.h>

class Dimension;
class LocationsRegistry
{
    NO_COPY_MOVE(LocationsRegistry);

public:
    LocationsRegistry(Dimension* dim);
    void initialize(const Config& config);

    Dimension& dimension();

    ObjectId resolveName(const std::string& name) const;

    const LocationInfoPtr& tryFind(ObjectId id);
    LocationInfo& find(ObjectId id);

    const std::vector<LocationInfoPtr>& locations() const;

    void reload(std::function<void(const std::string&)> callback);

private:
    void populateLocations(const Config& config);

private:
    Dimension* mDimension{nullptr};
    std::vector<LocationInfoPtr> mLocations;
};
