#pragma once

#ifndef DIMX_CORE_CONFIG_ARRAY_ITERATOR_H
#define DIMX_CORE_CONFIG_ARRAY_ITERATOR_H


#include <iterator>
#include <set>
#include <string>

class Config;
class ConfigArrayIterator
{
public:
    using difference_type   = int;
    using iterator_category = std::forward_iterator_tag;

public:
    ConfigArrayIterator(const Config& config, size_t index)
    : mConfig(config)
    , mIndex(index)
    {}

    ConfigArrayIterator(const ConfigArrayIterator& other) = default;
    ConfigArrayIterator(ConfigArrayIterator&& other) = default;

    ConfigArrayIterator operator++(int) { // iter++
        ConfigArrayIterator tmp = *this;
        operator++();
        return tmp;
    }

    ConfigArrayIterator operator++() { // ++iter
        ++mIndex;
        return *this;
    }

    bool operator==(const ConfigArrayIterator& rhs) const {
        return mIndex == rhs.mIndex;
    }

    bool operator!=(const ConfigArrayIterator& rhs) const {
        return !operator==(rhs);
    }

    std::pair<size_t, Config&> operator*();

private:
    const Config& mConfig;
    size_t mIndex = 0;
};

class ConfigArrayWrap {
public:
    ConfigArrayWrap(const Config& config): mConfig(config) {}
    ConfigArrayIterator begin() const { return {mConfig, 0}; }
    ConfigArrayIterator begin() { return {mConfig, 0}; }

    ConfigArrayIterator end() const;
    ConfigArrayIterator end();

private:
    const Config& mConfig;
};

#endif // DIMX_CORE_CONFIG_ARRAY_ITERATOR_H