#pragma once
#include <Common.h>
#include <config/Config.h>
#include <DoubleBuffer.h>
#include <mutex>
#include <thread>


STRUCT_DECLARE_PTR(ResourceResponse)
struct ResourceResponse {
    uint32_t requestId{0};
    std::string error;
    ConfigPtr data;
};

using ResponseCallback = std::function<void(const std::string&, ConfigPtr)>;

STRUCT_DECLARE_PTR(ResourceRequest)
struct ResourceRequest {
    uint32_t id{0};
    double timestamp{0.0};
    bool local{false};
    std::thread::id threadId;
    Config message;
    ResponseCallback callback;
};

class LocalResourceInterface;
class RemoteResourceInterface;
class ResourceCache;
class Settings;

class ResourceInterface
{
public:
    static std::vector<uint8_t> encodeRequest(const ResourceRequest& request);
    static ResourceResponsePtr decodeResponse(const char* data, size_t dataSize);

public:
    ResourceInterface(ConfigPtr engineConfig, const Settings* settings);
    ~ResourceInterface();

    void deinitialize();
    void pushEvent(Event event);
    void request(ConfigPtr message, ResponseCallback callback = {});
    void pushResponse(ResourceResponsePtr resp);

    const std::unique_ptr<ResourceCache>& cache() { return mCache; }

private:
    void threadFunc(ConfigPtr engineConfig, const Settings* settings);
    void initialize(ConfigPtr engineConfig, const Settings* settings);
    void processRequest(ConfigPtr message, ResponseCallback callback, std::thread::id threadId);
    void processResponse(ResourceResponsePtr resp);
    bool tryProcessRemoteClientMessage(ResourceResponsePtr resp);

private:
    std::atomic_bool mKeepRunning{true};
    std::thread mThread;

    std::mutex mMutex;
    std::condition_variable mConditionVar;

    DoubleBuffer<Event> mEvents;

    std::map<unsigned int, ResourceRequestPtr> mRequests;

    std::unique_ptr<LocalResourceInterface> mLocalInterface;
    std::unique_ptr<RemoteResourceInterface> mRemoteInterface;
    std::unique_ptr<ResourceCache> mCache;

    double mUpdateInterval = 1.0; // seconds
    double mNextUpdateTime = 0.0;
    double mRequestTimeout = 5.0; // seconds

    bool mOfflineMode = false;
};
