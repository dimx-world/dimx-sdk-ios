#pragma once

#ifndef DIMX_CORE_ENGINE_H
#define DIMX_CORE_ENGINE_H

#include "Common.h"
#include "FrameContext.h"
#include <config/Config.h>
#include "Settings.h"
#include "Counter.h"

#include <mutex>
#include <thread>

DECL_ENUM(EngineState,  PreInit,   Initializing,   InializingApp,   Running)
DECL_ESTR(EngineState, "PreInit", "Initializing", "InializingApp", "Running")

namespace ExecOpts
{
constexpr uint64_t None      = 0;
constexpr uint64_t AfterInit = 1ULL << 0;
}

namespace ui {
class ImGuiManager;
class UIScheme;
}

class Loader;
class ThreadPool;
class Clock;
class Display;
class PhysicsManager;
class World;
class Input;
class Camera;
class CameraCtrl;
class ResourceInterface;
class MultimediaManager;
class ResourceManager;
class GeolocationManager;
class AnalyticsManager;
class AccountManager;
class DeviceAR;
class Renderer;
class AudioDevice;
class AnchorManager;
class MaterialManager;
class AssetStore;
class FileDialog;

DECLARE_PTR(Engine);
class Engine
{
    NO_COPY_MOVE(Engine)

public:
    Engine(ConfigPtr config);
    virtual ~Engine();

    static bool valid() { return _instance != nullptr; }
    static Engine& inst() { return *_instance; }

    const Config& config() const { return *mConfig; }
    Settings& settings() { return mSettings; }

    std::thread::id threadId() const { return mThreadId; }

    void updateMain();
    virtual void processCommand(const std::string& command, const std::string& arguments = {}) {}
    EngineState currentState() const { return mCurrentState; }
    void pushEvent(Event event, uint64_t opts = 0);
    void processUrl(const std::string& url);

    Loader&            loader()            { return *mLoader; }
    ThreadPool&        threads()           { return *mThreadPool; }
    ResourceInterface& resourceInterface() { return *mResourceInterface; }
    Clock&             clock()             { return *mClock; }
    GeolocationManager& geolocation()      { return *mGeolocationManager; }
    AnalyticsManager&  analytics()         { return *mAnalyticsManager; }
    AccountManager&    account()           { return *mAccountManager; }
    Display&           display()           { return *mDisplay; }
    DeviceAR&          deviceAR()          { return *mDeviceAR; }
    AnchorManager&     anchors()           { return *mAnchorManager; }
    MultimediaManager& multimedia()        { return *mMultimediaManager; }
    ResourceManager&   resourceManager()   { return *mResourceManager; }
    MaterialManager&   materialManager()   { return *mMaterialManager; }
    AssetStore&        assetStore()        { return *mAssetStore; }
    Input&             input()             { return *mInput; }
    PhysicsManager&    physics()           { return *mPhysicsManager; }
    World&             world()             { return *mWorld; }
    Camera&            camera()            { return *mCamera; }
    CameraCtrl&        cameraCtrl()        { return *mCameraCtrl; }
    Renderer&          renderer()          { return *mRenderer; }
    AudioDevice&       audio()             { return *mAudioDevice; }
    ui::UIScheme&      uiScheme()          { return *mUIScheme; }
    ui::ImGuiManager&  imGuiManager()      { return *mImGuiManager; }
    FileDialog&        fileDialog()        { return *mFileDialog; }

    static AnalyticsManager* tryGetAnalytics() {
        return _instance ? _instance->mAnalyticsManager.get() : nullptr;
    }

    const FrameContext& currentFrameContext() const { return mFrameContext; }

    void deinitialize();

protected:
    void initialize();
    void update(const FrameContext& frameContext);
    void processEvents();
    void processAfterInitEvents();
    virtual void onStateChange(EngineState state) {}

private:
    void setCurrentState(EngineState state);
    void clearEvents();

private:
    std::thread::id    mThreadId;
    ConfigPtr          mConfig;
    Settings           mSettings;
    std::unique_ptr<Loader>            mLoader;
    std::unique_ptr<ThreadPool>        mThreadPool;
    std::unique_ptr<ResourceInterface> mResourceInterface;
    std::unique_ptr<Clock>             mClock;
    std::unique_ptr<GeolocationManager>mGeolocationManager;
    std::unique_ptr<AnalyticsManager>  mAnalyticsManager;
    std::unique_ptr<AccountManager>    mAccountManager;
    std::unique_ptr<Display>           mDisplay;
    std::unique_ptr<DeviceAR>          mDeviceAR;
    std::unique_ptr<AnchorManager>     mAnchorManager;
    std::unique_ptr<Input>             mInput;
    std::unique_ptr<Renderer>          mRenderer;
    std::unique_ptr<AudioDevice>       mAudioDevice;
    std::unique_ptr<Camera>            mCamera;
    std::unique_ptr<CameraCtrl>        mCameraCtrl;
    std::unique_ptr<MultimediaManager> mMultimediaManager;
    std::shared_ptr<ResourceManager>   mResourceManager;
    std::unique_ptr<MaterialManager>   mMaterialManager;
    std::unique_ptr<AssetStore>        mAssetStore;
    std::unique_ptr<PhysicsManager>    mPhysicsManager;
    std::unique_ptr<ui::UIScheme>      mUIScheme;
    std::unique_ptr<ui::ImGuiManager>  mImGuiManager;
    std::unique_ptr<World>             mWorld;
    std::unique_ptr<FileDialog>        mFileDialog;

private:
    static Engine* _instance;
    EngineState mCurrentState = EngineState::None;

    FrameContext mFrameContext;

    std::mutex mEventsLock;
    std::vector<Event> mEvents;
    std::vector<Event> mAfterInitEvents;

    CounterPtr mInitCounter;
};

inline Engine&          g_engine()      { return Engine::inst();              }
inline const Config&    g_config()      { return g_engine().config();         }
inline Settings&        g_settings()    { return g_engine().settings();       }
inline Loader&          g_loader()      { return g_engine().loader();         }
inline ThreadPool&      g_threads()     { return g_engine().threads();        }
inline ResourceInterface& g_resourceInterface() { return g_engine().resourceInterface(); }
inline Clock&           g_clock()       { return g_engine().clock();          }
inline GeolocationManager& g_geolocation() { return g_engine().geolocation(); }
inline AnalyticsManager& g_analytics()  { return g_engine().analytics();      }
inline AccountManager&  g_account()     { return g_engine().account();        }
inline Display&         g_display()     { return g_engine().display();        }
inline DeviceAR&        g_deviceAR()    { return g_engine().deviceAR();       }
inline AnchorManager&   g_anchors()     {return g_engine().anchors();         }
inline MultimediaManager& g_multimedia() {return g_engine().multimedia();     }
inline ResourceManager& g_resourceManager() { return g_engine().resourceManager(); }
inline MaterialManager& g_materialManager() { return g_engine().materialManager(); }
inline AssetStore&      g_assetStore()  { return g_engine().assetStore();     }
inline Input&           g_input()       { return g_engine().input();          }
inline PhysicsManager&  g_physics()     { return g_engine().physics();        }
inline World&           g_world()       { return g_engine().world();          }
inline Camera&          g_camera()      { return g_engine().camera();         }
inline Renderer&        g_renderer()    { return g_engine().renderer();       }
inline AudioDevice&     g_audio()       { return g_engine().audio();          }
inline ui::ImGuiManager& g_imgui()      { return g_engine().imGuiManager();   }

#define ASSERT_MAIN_THREAD()                                    \
    ASSERT(std::this_thread::get_id() == g_engine().threadId(), \
           "Must be called on the main thread! "                \
           << "This thread " << std::this_thread::get_id()      \
           << ", main thread " << g_engine().threadId())

#endif // DIMX_CORE_ENGINE_H