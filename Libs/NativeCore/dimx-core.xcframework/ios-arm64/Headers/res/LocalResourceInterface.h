#pragma once
#include "ClientResourceInterface.h"

class LocalServerResourceInterface;
class LocalResourceInterface: public ClientResourceInterface
{
public:
    LocalResourceInterface();
    virtual ~LocalResourceInterface();

    void initialize(const Settings& settings, const Config& config) override;
    void sendRequest(ResourceRequestPtr request) override;

    void processLocalResponse(ResourceResponsePtr resp);

private:
    std::shared_ptr<LocalServerResourceInterface> mServer;
};
