#pragma once

#ifndef DIMX_CORE_OBJECT_ID_H
#define DIMX_CORE_OBJECT_ID_H

#include <string>
#include <iosfwd>
#include <functional>

class ObjectId
{

private:
    // For internal use only
    constexpr ObjectId(uint64_t val): mValue(val) {}

public:
    constexpr ObjectId() = default;
    constexpr ObjectId(const ObjectId&) = default;
    constexpr ObjectId& operator=(const ObjectId&) = default;

    constexpr static ObjectId fromUInt64(uint64_t val) { return ObjectId(val); }
    constexpr uint64_t toUInt64() const { return mValue; }

    static ObjectId fromString(const std::string& str);
    std::string toString() const;

    static ObjectId random();

    constexpr bool isNull() const { return mValue == 0; }
    constexpr operator bool() const { return !isNull(); }

    constexpr bool operator == (const ObjectId& other) const { return mValue == other.mValue; }
    constexpr bool operator != (const ObjectId& other) const { return mValue != other.mValue; }
    constexpr bool operator <  (const ObjectId& other) const { return mValue <  other.mValue; }
    constexpr bool operator >  (const ObjectId& other) const { return mValue >  other.mValue; }
    constexpr bool operator <= (const ObjectId& other) const { return mValue <= other.mValue; }
    constexpr bool operator >= (const ObjectId& other) const { return mValue >= other.mValue; }

private:
    uint64_t mValue = 0;
};

std::ostream& operator << (std::ostream& os, const ObjectId& id);

namespace std {
template <> struct hash<ObjectId> {
    size_t operator()(const ObjectId& id) const {
        return id.toUInt64();
    }
};
}

#endif // DIMX_CORE_OBJECT_ID_H