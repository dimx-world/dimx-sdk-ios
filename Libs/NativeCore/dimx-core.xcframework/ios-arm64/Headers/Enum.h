#pragma once

#ifndef DIMX_CORE_ENUM_H
#define DIMX_CORE_ENUM_H

#include <string>
#include <iosfwd>

template <typename T>
const std::string& enum2str(const T enumVal);

template <typename T>
T str2enum(const std::string& str);

/*
// Unfortunately this won't work in Visual Studio
// because Microsoft bastards don't comply with the standard.
// At some point it starts treating __VA_ARGS__ as one argument
// "1, 2, 3" instead of 1, 2, 3
#define t0
#define t1(x1) #x1
#define t2(x1, x2) #x1, #x2
#define t3(x1, x2, x3) #x1, #x2, #x3
#define t4(x1, x2, x3, x4) #x1, #x2, #x3, #x4
#define t5(x1, x2, x3, x4, x5) #x1, #x2, #x3, #x4, #x5

#define GET_TOKENIZER(...) GET_TOKENIZER_IMPL(__VA_ARGS__, t5, t4, t3, t2, t1)
#define GET_TOKENIZER_IMPL(_1, _2, _3, _4, _5, N, ...) N
#define TOKENIZE(...) GET_TOKENIZER(__VA_ARGS__)(__VA_ARGS__)

// TODO: replace DECL_ENUM_STRINGS_PLACEHOLDER with the code below
// when VS learns how to handle macros
inline std::string* EnumName##_strs()  \
{                                      \
static std::string _strings[] = {      \
    TOKENIZE(__VA_ARGS__), "None"      \
};                                     \
return _strings;                       \
}                                      \
*/
#define DECL_ENUM_STRINGS_PLACEHOLDER

//--------------------------------------------------------
#define DECL_ENUM(EnumName, ...)                         \
enum class EnumName {__VA_ARGS__, None, Max = None};     \
                                                         \
DECL_ENUM_STRINGS_PLACEHOLDER                            \
                                                         \
std::string* EnumName##_strs();                          \
                                                         \
template <>                                              \
inline                                                   \
const std::string& enum2str(const EnumName enumVal)      \
{                                                        \
    return EnumName##_strs()[                            \
        (size_t)enumVal < (size_t)EnumName::Max          \
        ? (size_t)enumVal : (size_t)EnumName::Max ];     \
}                                                        \
                                                         \
template <>                                              \
inline                                                   \
EnumName str2enum<EnumName>(const std::string& str)      \
{                                                        \
    for (size_t i = 0; i < (size_t)EnumName::Max; ++i) { \
        if (EnumName##_strs()[i] == str)                 \
            return (EnumName)i;                          \
    }                                                    \
    return EnumName::None;                               \
}                                                        \
                                                         \
inline                                                   \
std::ostream& operator<< (std::ostream& os, EnumName val)\
{                                                        \
    return os << enum2str(val);                          \
}

//--------------------------------------------------------
#define DECL_ESTR(EnumName, ...)                         \
inline std::string* EnumName##_strs()                    \
{                                                        \
    static std::string _strs[] = {__VA_ARGS__, "None"};  \
    return _strs;                                        \
}

#endif // DIMX_CORE_ENUM_H
