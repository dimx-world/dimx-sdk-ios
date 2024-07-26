#pragma once
#include "LoaderJob.h"

class LocationLoader;
class DimensionLoader: public std::enable_shared_from_this<DimensionLoader>
{

public:
    static std::shared_ptr<DimensionLoader> create(ObjectId id) {
        return std::make_shared<DimensionLoader>(id);
    }

public: // should be private
    DimensionLoader(ObjectId id);

public:
    void initialize();
    
    const DimensionPtr& dimension() {
        ASSERT(mDimension, "Dimension is not initialized yet");
        return mDimension;
    }

    void enterLocation(ObjectId locId);
    void exitLocation(ObjectId locId);

private:
    void deliverConfig(const std::string& error, ConfigPtr config);

private:
    ObjectId mDimensionId;
    ConfigPtr mConfig;
    DimensionPtr mDimension;

    std::map<ObjectId, std::shared_ptr<LocationLoader>> mLocations;
};
