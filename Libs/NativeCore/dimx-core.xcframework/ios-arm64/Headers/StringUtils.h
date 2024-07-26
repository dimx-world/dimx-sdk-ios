#pragma once

#ifndef DIMX_CORE_STRING_UTILS_H
#define DIMX_CORE_STRING_UTILS_H

#include "Enum.h"
#include <string>
#include <vector>

bool strEndsWith(const std::string& str, const std::string& ending);
bool strStartsWith(const std::string& str, const std::string& beginning);

std::vector<std::string> strSplit(const std::string& input, char delimiter);

std::string strBaseName(const std::string& fullPath);
std::string strFileName(const std::string& fullPath);
std::string strDirPath(const std::string& fullPath);
std::string strReplaceFileExt(const std::string& fullPath, const std::string& ext);
std::string strPathRemoveEndingSlash(const std::string& fullPath);

DECL_ENUM(MessageTag,  PENDING,   ERROR);
DECL_ESTR(MessageTag, "PENDING", "ERROR");

std::string strAddMessageTag(const std::string& text, MessageTag tag);
MessageTag strParseMessageTag(const std::string& text);
std::string strRemoveMessageTag(const std::string& text);

template <typename T> std::string toStr(const T& val);
template <typename T> T fromStr(const std::string& str);

#endif // DIMX_CORE_STRING_UTILS_H