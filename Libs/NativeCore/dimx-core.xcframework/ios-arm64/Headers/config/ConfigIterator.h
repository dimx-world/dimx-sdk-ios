#pragma once

#ifndef DIMX_CORE_CONFIG_ITERATOR_H
#define DIMX_CORE_CONFIG_ITERATOR_H

#include <iterator>
#include <map>
#include <string>
#include <vector>

class Config;
class ConfigIterator
{
public:
    enum InitMode {Begin, End};

    using difference_type   = int;
    using iterator_category = std::forward_iterator_tag;

    using ArrayIter = std::vector<Config>::const_iterator;
    using DictIter = std::map<std::string, Config>::const_iterator;
public:
    ConfigIterator(const Config& config, InitMode mode);

    ConfigIterator(const ConfigIterator& other) = default;
    ConfigIterator(ConfigIterator&& other) = default;

    ConfigIterator operator++(int) { // iter++
        ConfigIterator tmp = *this;
        operator++();
        return tmp;
    }

    ConfigIterator operator++();// ++iter

    bool operator==(const ConfigIterator& rhs) const;

    bool operator!=(const ConfigIterator& rhs) const {
        return !operator==(rhs);
    }

    Config& operator*();

private:
    const Config& mConfig;
    ArrayIter mArrayIter;
    DictIter mDictIter;
};

#endif // DIMX_CORE_CONFIG_ITERATOR_H