#pragma once
#include "ResourceInterface.h"
#include <CrossObject.h>

class LocalResourceInterface;
class LocalServerResourceInterface: public CrossObject
{
public:
    LocalServerResourceInterface();
    ~LocalServerResourceInterface();
    void processLocalRequest(LocalResourceInterface& client, ResourceRequestPtr request);

    virtual void initialize(const Config& config);

private:
    ConfigPtr processGetDimension(const Config& request);
    ConfigPtr processGetLocation(const Config& request);
    ConfigPtr processGetFile(const Config& request);

private:
    std::string mBaseDir;
};
