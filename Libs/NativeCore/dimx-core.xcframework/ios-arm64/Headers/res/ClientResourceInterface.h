#pragma once
#include "Common.h"
#include <CrossObject.h>
#include "ResourceInterface.h"
#include <config/Config.h>

class Settings;

DECLARE_PTR(ClientResourceInterface)
class ClientResourceInterface: public CrossObject
{
public:
    ClientResourceInterface() {};
    virtual ~ClientResourceInterface() {};
    virtual void initialize(const Settings& settings, const Config& config) {};
    virtual void sendRequest(ResourceRequestPtr request) {};
    virtual bool flushRequests(double timeoutSecs) { return true; };
};
