#pragma once

#ifndef DIMX_CORE_APP_UTILS_H
#define DIMX_CORE_APP_UTILS_H

#include <FileSystem.h>
#include <Settings.h>
#include <account/AccountManager.h>
#include <config/ConfigUtils.h>

namespace AppUtils {

template <typename T>
std::unique_ptr<T> createEngine(const std::string& appDataPath,
                                const std::string& localStoragePath,
                                const std::string& cachePath,
                                const std::string& extMediaPath,
                                const std::string& url,
                                const std::string& settingsData,
                                const std::string& accountData)
{
    g_fileSystem().initialize(appDataPath, localStoragePath, cachePath, extMediaPath);

    if (!settingsData.empty()) {
        Settings::saveConfigData(settingsData);
    }

    if (!accountData.empty()) {
        AccountManager::saveAccountData(accountData);
    }

    ConfigPtr config = std::make_shared<Config>(ConfigUtils::loadFromFile(appDataPath + "/engine/engine.json"));
    ConfigUtils::processIncludes(*config, appDataPath + "/engine");
    ConfigUtils::loadFilesListNode(*config, "files", "engine");

    auto engine = std::make_unique<T>(config);

    if (!url.empty()) {
        g_engine().processUrl(url);
    }

    return engine;
}

void mobileSessionReload(const std::string& url,
                         const std::string& settingsData,
                         const std::string& accountData);

void mobileSessionEnd();

std::string convertAppUrlToWebUrl(const std::string& webAppHost, const std::string& appUrl);

} // namespace AppUtils 

#endif // DIMX_CORE_APP_UTILS_H
