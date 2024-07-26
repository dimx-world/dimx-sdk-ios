#pragma once
#include <res/RemoteResourceInterface.h>
#include <thread>
#include <mutex>
#include <chrono>

class WebResourceInterface: public RemoteResourceInterface
{
    static constexpr int ThreadSleepMS = 100;
    static constexpr int ReconnectIntervalMS = 1000;
public:
    WebResourceInterface();
    virtual ~WebResourceInterface();

    void initialize(const Settings& settings, const Config& config) override;

    void sendRequest(ResourceRequestPtr request) override;
    bool flushRequests(double timeoutSecs) override;

private:
    void openConnection();
    void closeConnection();
    void backgroundThread();
    void handleMessage(const std::vector<uint8_t>& message);

    void setConnected(bool connected);

private:
    std::mutex mRequestsLock;
    std::vector<ResourceRequestPtr> mRequests;

    bool mKeepRunning = true;
    std::thread mThread;

    std::string mConnectionString;
    std::atomic<bool> mConnected{false};
};
