#pragma once

#ifndef DIMX_CORE_CONFIG_PARSER_H
#define DIMX_CORE_CONFIG_PARSER_H

#include "Common.h"

class Config;
class ConfigParser
{
public:
    ConfigParser();
    virtual ~ConfigParser();

    bool parse(const std::string& filePath, Config& config) const;
    bool parse(const char* text, const size_t size, Config& config) const;
};

#endif // DIMX_CORE_CONFIG_PARSER_H