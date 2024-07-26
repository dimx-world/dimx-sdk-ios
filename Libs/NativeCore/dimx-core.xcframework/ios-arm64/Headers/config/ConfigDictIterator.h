#pragma once

#ifndef DIMX_CORE_CONFIG_DICT_ITERATOR_H
#define DIMX_CORE_CONFIG_DICT_ITERATOR_H

#include <iterator>
#include <map>
#include <string>

class Config;
class ConfigDictIterator
{
public:
    using difference_type   = int;
    using iterator_category = std::forward_iterator_tag;

    using Iter = std::map<std::string, Config>::const_iterator;

public:
    ConfigDictIterator(Iter iter)
    : mIter(iter)
    {}

    ConfigDictIterator(const ConfigDictIterator& other) = default;
    ConfigDictIterator(ConfigDictIterator&& other) = default;

    ConfigDictIterator operator++(int) { // iter++
        ConfigDictIterator tmp = *this;
        operator++();
        return tmp;
    }

    ConfigDictIterator operator++(); // ++iter

    bool operator==(const ConfigDictIterator& rhs) const;

    bool operator!=(const ConfigDictIterator& rhs) const {
        return !operator==(rhs);
    }

    std::pair<const std::string&, Config&> operator*();

private:
    Iter mIter;
};

class ConfigDictWrap {
public:
    ConfigDictWrap(const Config& config): mConfig(config) {}
    ConfigDictIterator begin() const;
    ConfigDictIterator begin();

    ConfigDictIterator end() const;
    ConfigDictIterator end();

private:
    const Config& mConfig;
};

#endif // DIMX_CORE_CONFIG_DICT_ITERATOR_H