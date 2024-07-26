#pragma once
#include "Common.h"
#include "Enum.h"
#include "StringUtils.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

DECL_ENUM(CerealType,  Binary,   Json,   Xml)
DECL_ESTR(CerealType, "Binary", "Json", "Xml")

inline CerealType getCerealType(const std::string& filepath)
{
    if (strEndsWith(filepath, ".json")) {
        return CerealType::Json;
    } else if (strEndsWith(filepath, ".xml")) {
        return CerealType::Xml;
    }
    return CerealType::Binary;
}

#define LOAD_ARCHIVE_NVP(NAME, VAL, IS, TYPE)        \
    switch (TYPE) {                                  \
        case CerealType::Json: {                     \
            cereal::JSONInputArchive archive(IS);    \
            ARCHIVE_NVP(archive, NAME, VAL);         \
            break;                                   \
        }                                            \
        case CerealType::Xml: {                      \
            cereal::XMLInputArchive archive(IS);     \
            ARCHIVE_NVP(archive, NAME, VAL);         \
            break;                                   \
        }                                            \
        case CerealType::Binary:                     \
        default: {                                   \
            cereal::BinaryInputArchive archive(IS);  \
            ARCHIVE_NVP(archive, NAME, VAL);         \
        }                                            \
    }

#define LOAD_ARCHIVE(VAL, IS, TYPE)  LOAD_ARCHIVE_NVP(VAL, VAL, IS, TYPE)

///////////////////////////////////////////////////////////////////////////////////////////////

inline void cereal_save_binary(cereal::BinaryOutputArchive & ar, const void* data, size_t size)
{
    ar.saveBinary(data, size);
}

inline void cereal_load_binary(cereal::BinaryInputArchive & ar, void* data, size_t size)
{
    ar.loadBinary(data, size);
}

inline void cereal_save_binary(cereal::JSONOutputArchive & ar, const void* data, size_t size)
{
    ar.saveBinaryValue(data, size);
}

inline void cereal_load_binary(cereal::JSONInputArchive & ar, void* data, size_t size)
{
    ar.loadBinaryValue(data, size);
}

inline void cereal_save_binary(cereal::XMLOutputArchive & ar, const void* data, size_t size)
{
    ar.saveBinaryValue(data, size);
}

inline void cereal_load_binary(cereal::XMLInputArchive & ar, void* data, size_t size)
{
    ar.loadBinaryValue(data, size);
}

