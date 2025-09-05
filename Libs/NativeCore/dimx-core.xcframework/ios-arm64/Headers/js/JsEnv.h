#pragma once
#include "Common.h"
#include <quickjspp.hpp>
#include <thread>
#include <mutex>
#include <unordered_map>

class Config;
class Buffer;
DECLARE_PTR(JsEnv)
class JsEnv
{
    using FileLoader = std::function<std::shared_ptr<Buffer>(const std::string&)>;
public:
    JsEnv(const std::string& name, FileLoader loader = {});

    qjs::Value eval(std::string_view buffer, const char * filename = "<eval>", unsigned int flags = 0);
    qjs::Value evalFile(const std::string& filename, unsigned int flags = JS_EVAL_TYPE_MODULE);

    qjs::Context::Module& dimxModule() {
        ASSERT(std::this_thread::get_id() == mCreationThreadId, "JsEnv bad thread: " << std::this_thread::get_id());
        ASSERT(mContext.modules.size() == 1, "Expected only one module in js context!. Actual: " << mContext.modules.size());
        return mContext.modules[0];
    };

    JSContext* jsContext() const {
        //ASSERT(std::this_thread::get_id() == mCreationThreadId, "JsEnv bad thread: " << std::this_thread::get_id());
        return mContext.ctx;
    }

    qjs::Value global() { return mContext.global(); }

    qjs::Value newObject() { return mContext.newObject(); }

    // Register a cluster module that can be imported by name
    void registerClusterModule(const std::string& clusterName, const std::string& filePath);
    
    // Get the file path for a registered cluster module (used by module loader)
    std::string getClusterModulePath(const std::string& clusterName) const;

    // Unprotected calls!
    Config jsValueToConfig(qjs::Value value);         // these calls are originated from javascript
    qjs::Value configToJsValue(const Config& config); // must always be done within safeCall()

    template <typename Call, typename... Args>
    void safeCall(Call call, Args&&... args) {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        try {
            call(std::forward<decltype(args)>(args)...);
            if (!JS_IsNull(JS_PeekException(mContext.ctx))) {
                handleCurrentException();
            }
        } catch (qjs::exception) {
            handleCurrentException();
        }
    }

    template <typename Call, typename... Args>
    std::invoke_result_t<Call> safeCallRet(Call call, Args&&... args) {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        try {
            auto res = call(std::forward<decltype(args)>(args)...);
            if (!JS_IsNull(JS_PeekException(mContext.ctx))) {
                handleCurrentException();
                return {};
            }
            return res;
        } catch (qjs::exception) {
            handleCurrentException();
        }
        return {};
    }

private:
    std::string loadTextFile(const std::string& filePath);
    void handleCurrentException(size_t depth = 0);

private:
    std::thread::id mCreationThreadId;
    std::recursive_mutex mMutex;
    FileLoader mFileLoader;
    qjs::Runtime mRuntime;
    qjs::Context mContext;
    qjs::Value mDateCtr;
    
    // Registry for cluster modules that can be imported by name
    std::unordered_map<std::string, std::string> mClusterModules;
};
