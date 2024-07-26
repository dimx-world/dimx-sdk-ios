#pragma once
#include <Common.h>
#include <config/Config.h>

class DimensionLoader;
class Location;
class LocationLoader: public std::enable_shared_from_this<LocationLoader>
{

public:
    static std::shared_ptr<LocationLoader> create(DimensionLoader* parent, ObjectId locId) {
        return std::make_shared<LocationLoader>(parent, locId);
    }

public: // should be private
    LocationLoader(DimensionLoader* parent, ObjectId locId);

public:
    void initialize();

private:
    void deliverConfig(const std::string& error, ConfigPtr config);

private:
    DimensionLoader* mParent{nullptr};
    ObjectId mLocationId;
    ConfigPtr mConfig;
    std::shared_ptr<Location> mLocation;
};
