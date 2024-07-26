#pragma once
#include "VertexAttribute.h"

class VertexFormat
{
    // for internal use only
    VertexFormat(uint64_t attributes): mAttributes(attributes) {}

public:
    VertexFormat() = default;
    VertexFormat(const VertexFormat& other) = default;
    VertexFormat& operator= (const VertexFormat& other)
    {
        mAttributes = other.mAttributes;
        return *this;
    }

    static VertexFormat fromString(const std::string& str)
    {
        uint64_t attribs = 0;
        if (!str.empty()) {
            const char* begin = str.c_str();
            char* end = nullptr;
            attribs = std::strtoull(begin, &end, 10);
            ASSERT(end - begin == str.size(), "Invalid VertexFormat [" << str << "]");
        }
        return VertexFormat(attribs);
    }

    void reset()
    {
        mAttributes = 0;
    }

    bool operator== (const VertexFormat& other) const
    {
        return mAttributes == other.mAttributes;
    }

    bool operator< (const VertexFormat& other) const
    {
        return mAttributes < other.mAttributes;
    }

    void addAttribute(VertexAttribType attrib)
    {
        mAttributes |= (1ull << static_cast<int>(attrib));
    }

    bool hasAttribute(VertexAttribType attrib) const
    {
        return mAttributes & (1ull << static_cast<int>(attrib));
    }

    std::string toString() const
    {
        return std::to_string(mAttributes);
    }

    bool valid() const
    {
        return mAttributes != 0;
    }

    operator bool() const
    {
        return valid();
    }
private:
    uint64_t mAttributes{0};
};
