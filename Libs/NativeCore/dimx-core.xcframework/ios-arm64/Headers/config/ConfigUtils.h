#pragma once

#ifndef DIMX_CORE_CONFIG_UTILS_H
#define DIMX_CORE_CONFIG_UTILS_H

#include "Config.h"

namespace ConfigUtils {

Config parseText(const char* text, const size_t size);
Config loadFromFile(const std::string& filepath);
void processIncludes(Config& node, const std::string& lookupPath);
void loadNodeFromFile(Config& config, const std::string& node, const std::string& path);
void loadFilesListNode(Config& config, const std::string& node, const std::string& baseDir);

} // namespace ConfigUtils

#endif // DIMX_CORE_CONFIG_UTILS_H