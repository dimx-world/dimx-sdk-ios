#pragma once

#ifndef DIMX_CORE_CONFIG_H
#define DIMX_CORE_CONFIG_H

#include <Common.h>
#include "ConfigIterator.h"
#include "ConfigDictIterator.h"
#include "ConfigArrayIterator.h"
#include <Buffer.h>
#include <DateTime.h>

#include <string>
#include <vector>
#include <any>

DECL_ENUM(ConfigValueType,  Null,   Boolean,   Integer,   Real,   String,   DateTime,   Any,   Array,   Dict);
DECL_ESTR(ConfigValueType, "Null", "Boolean", "Integer", "Real", "String", "DateTime", "Any", "Array", "Dict");

class OBufferStream;
class IBufferStream;
class Config
{
    friend class ConfigIterator;
    friend class ConfigArrayIterator;
    friend class ConfigArrayWrap;
    friend class ConfigDictIterator;
    friend class ConfigDictWrap;

    static constexpr uint32_t BINARY_TAG = 111222333;
    static constexpr uint32_t BINARY_VER = 0;

public:
    using Null = std::monostate;
    using Boolean = bool;
    using Integer = long;
    using Real = double;
    using String = std::string;

    using Any = std::any;
    using Array = std::vector<Config>;
    using Dict = std::map<std::string, Config>;
    using Value = std::variant<Null, Boolean, Integer, Real, String, DateTime, Any, Array, Dict>;

public:
    bool isNull()     const { return std::holds_alternative<Null>(mValue) || mValue.valueless_by_exception(); }
    bool isBoolean()  const { return std::holds_alternative<Boolean>(mValue); }
    bool isInteger()  const { return std::holds_alternative<Integer>(mValue); }
    bool isReal()     const { return std::holds_alternative<Real>(mValue); }
    bool isString()   const { return std::holds_alternative<String>(mValue); }
    bool isDateTime() const { return std::holds_alternative<DateTime>(mValue); }
    bool isAny()      const { return std::holds_alternative<Any>(mValue); }
    bool isArray()    const { return std::holds_alternative<Array>(mValue); }
    bool isDict()     const { return std::holds_alternative<Dict>(mValue); }

    std::string toString() const;

    // Getters
    template <typename T = std::string,
              typename std::enable_if<!std::is_enum<T>::value>::type* = nullptr>
    T get() const  { return getValue<T>(); }

    template <typename T = std::string,
              typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    T get() const { return str2enum<T>(getValue<String>()); }

    template <typename T = std::string>
    T get(const std::string& key) const  { return getNode(key).get<T>(); }

    template <typename T>
    T get(const std::string& key, const T& defaultValue) const {
        const Config* node = tryGetNode(key);
        return node ? node->get<T>() : defaultValue;
    }

    std::string get(const std::string& key, const char* defaultValue) const {
        return get(key, std::string(defaultValue));
    }

    // Setters
    template <typename T,
              typename std::enable_if<!std::is_enum<T>::value>::type* = nullptr>
    Config& set(T value) { setValue(std::move(value)); return* this; }
    
    template <typename T,
              typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    Config& set(T value) { return set(enum2str(value)); }

    template <typename T> Config& set(const std::string& key, T value) {
        return getOrCreateNode(key).set(std::move(value));
    }

    Config& set(const std::string& key, const char* value) {
        return set(key, std::string(value));
    }

    //--------------------------------------------------------------------------
    // TODO: implement simple get<> with std::enable_if for any
    template <typename T>
    const T& getAny() const {
        return std::any_cast<const T&>(std::get<Any>(mValue));
    }

    template <typename T>
    const T& getAny(const std::string& key) const {
        return getNode(key).getAny<T>();
    }

    template <typename T>
    const T& getAny(const std::string& key, const T& defaultValue) const {
        const Config* node = tryGetNode(key);
        return node ? node->getAny<T>() : defaultValue;
    }

    template <typename T>
    Config& setAny(T value) {
        return set(std::any{std::move(value)});
    }
    template <typename T>
    Config& setAny(const std::string& key, T value) {
        return getOrCreateNode(key).setAny(std::move(value));
    }
    //--------------------------------------------------------------------------

    ConfigIterator begin() const { return ConfigIterator(const_cast<Config&>(*this), ConfigIterator::InitMode::Begin); }
    ConfigIterator begin() { return ConfigIterator(*this, ConfigIterator::InitMode::Begin); }
    ConfigIterator end() const { return ConfigIterator(const_cast<Config&>(*this), ConfigIterator::InitMode::End); }
    ConfigIterator end() { return ConfigIterator(*this, ConfigIterator::InitMode::End); }

    ConfigArrayWrap array() const { return {*this}; }
    ConfigDictWrap dict() const { return {*this}; }

    bool contains(const std::string& key) const { return tryGetNode(key) != nullptr; }

    template <typename T> Config& pushBack(const T& item);
    Config& pushBack(Config config);
    size_t size() const;

    size_t erase(const std::string& name);

    size_t writeToBuffer(Buffer& buffer) const;
    void readFromBuffer(const char* buff, size_t size);

    ConfigValueType getValueType() const;
    std::string getInfo() const;

private:
    void validateVariantEnum() const;
    void writeToBufferStream(OBufferStream& stream) const;
    void readFromBufferStream(IBufferStream& stream, uint32_t version);

    Config* tryGetNode(const std::string& key);
    Config* tryGetNode(const std::string& key) const {
        return const_cast<Config&>(*this).tryGetNode(key);
    }
    Config& getNode(const std::string& key) ;
    Config& getNode(const std::string& key) const {
        return const_cast<Config&>(*this).getNode(key);
    }
    Config& getOrCreateNode(const std::string& key);

    template <typename T> void setValue(T value);
    template <typename T = std::string> T getValue() const;

    void print(std::ostream& os) const;

private:
    Value mValue;
};

inline
std::ostream& operator<<(std::ostream& os, const Config& config)
{
    return os << config.toString();
}

template <typename T> Config& Config::pushBack(const T& item)
{
    if (isNull()) {
        mValue = Array{};
    }
    ASSERT(isArray(), "Can't push into non-array config!");
    return std::get<Array>(mValue).emplace_back().set(item);
}

inline Config& Config::pushBack(Config config)
{
    if (isNull()) {
        mValue = Array{};
    }
    ASSERT(isArray(), "Can't push into non-array config!");
    std::get<Array>(mValue).emplace_back(std::move(config));
    return std::get<Array>(mValue).back();
}

template <> inline Config* Config::get<Config*>(const std::string& key) const
{
    return get<Config*>(key, nullptr);
}

#endif // DIMX_CORE_CONFIG_H