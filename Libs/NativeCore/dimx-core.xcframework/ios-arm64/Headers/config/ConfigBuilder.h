#pragma once

#ifndef DIMX_CORE_CONFIG_BUILDER_H
#define DIMX_CORE_CONFIG_BUILDER_H

#include "Config.h"

class ConfigBuilder
{
public:
    static ConfigBuilder makeShared() {
        return ConfigBuilder(std::make_shared<Config>());
    }

public:
    ConfigBuilder(std::shared_ptr<Config> shared)
        : ConfigBuilder(*shared, nullptr)
    {
        mSharedPtr = std::move(shared);
    }

    ConfigBuilder(Config& cfg, const ConfigBuilder* parent)
        : mConfig(cfg)
        , mParent(parent)
    {}

    template <typename T>
    ConfigBuilder set(const std::string& key, T value) const {
        mConfig.set(key, std::move(value));
        return *this;
    }

    ConfigBuilder set(const std::string& key, Config::Dict value) const {
        Config& cfg = mConfig.set(key, std::move(value));
        return ConfigBuilder(cfg, this);
    }

    ConfigBuilder set(const std::string& key, Config::Array value) const {
        Config& cfg = mConfig.set(key, std::move(value));
        return ConfigBuilder(cfg, this);
    }

    ConfigBuilder set(const std::string& key, Config config) const {
        mConfig.set(key, std::move(config));
        return *this;
    }

    ConfigBuilder set(const std::string& key, const char* value) const {
        return set(key, std::string(value));
    }

    template <typename T>
    ConfigBuilder setAny(const std::string& key, T value) const {
        mConfig.setAny(key, std::move(value));
        return *this;
    }

    template <typename T>
    ConfigBuilder push(const T& item) const {
        Config& cfg = mConfig.pushBack(item);
        return cfg.isDict() || cfg.isArray() ? ConfigBuilder(cfg, this) : *this;
    }

    ConfigBuilder push(Config config) const {
        mConfig.pushBack(std::move(config));
        return *this;
    }

    ConfigBuilder back() const {
        ASSERT(mParent, "null parent");
        return *mParent;
    }

    std::shared_ptr<Config> end() const {
        if (mParent) {
            return mParent->end();
        }
        ASSERT(mSharedPtr, "Null shared ptr in ConfigBuilder")
            return mSharedPtr;
    }

private:
    Config& mConfig;
    const ConfigBuilder* mParent{ nullptr };
    std::shared_ptr<Config> mSharedPtr;
};

/*
    auto config = ConfigUtils::makeShared()
        .set("name", std::string("Video-") + toStr(random(1000, 9999)))
        .set("persistent", true)
        .set(Tag, Config::Null{})
        .set(ModelNode::Tag, Config::Dict{})
            .set("model", "video-frame")
            .set("material", Config::Dict{})
                .set("base", "Standard")
                .set("overrides", Config::Dict{})
                    .set("base_color_map", "video-frame")
                    .parent()
                .parent()
            .end();

auto Config::build()
    .set("dfdf", 12)
    .set("sdsdf", Config::Dict())
        .set("sadfsdf", 34)
        .parent()
    .set("sdfsdf", Config::Null{})
    .set("aaaa", Config::Dict{})
        .parent()
    .set("sdfasdf", Config::Array{})
        .pushBack("", 12312)
        .pushBack("", 12312);
*/

#endif // DIMX_CORE_CONFIG_BUILDER_H