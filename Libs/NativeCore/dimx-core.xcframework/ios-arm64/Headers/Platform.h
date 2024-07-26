#pragma once

#ifndef DIMX_CORE_PLATFORM_H
#define DIMX_CORE_PLATFORM_H

#if defined(DIMX_PLATFORM_WINDOWS)
  static constexpr const char* DIMX_PLATFORM = "Windows";
  #define DIMX_PLATFORM_DESKTOP
#elif defined(DIMX_PLATFORM_WEB)
  static constexpr const char* DIMX_PLATFORM = "Web";
  #define DIMX_PLATFORM_DESKTOP
#elif defined(DIMX_PLATFORM_LINUX)
  static constexpr const char* DIMX_PLATFORM = "Linux";
  #define DIMX_PLATFORM_DESKTOP
#elif defined (DIMX_PLATFORM_ANDROID)
  static constexpr const char* DIMX_PLATFORM = "Android";
  #define DIMX_PLATFORM_MOBILE
  #define DIMX_PLATFORM_XR
#elif defined (DIMX_PLATFORM_IOS)
  static constexpr const char* DIMX_PLATFORM = "iOS";
  #define DIMX_PLATFORM_MOBILE
  #define DIMX_PLATFORM_XR
#elif defined(DIMX_PLATFORM_MAGICLEAP)
  static constexpr const char* DIMX_PLATFORM = "MagicLeap";
  #define DIMX_PLATFORM_XR
#else
  static_assert(false, "Unknown platform");
#endif

#endif // DIMX_CORE_PLATFORM_H