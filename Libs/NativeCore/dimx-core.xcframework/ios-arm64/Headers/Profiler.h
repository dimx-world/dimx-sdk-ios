#pragma once

#ifndef DIMX_CORE_PROFILER_H
#define DIMX_CORE_PROFILER_H

#ifdef PROFILER_ENABLED

#include <tracy/Tracy.hpp>

#define PROFILER_THREAD(name) tracy::SetThreadName(name)
#define PROFILER_FRAME_END() FrameMark
#define PROFILER_SCOPE(name) ZoneScopedN(name)

#else

#define PROFILER_THREAD(name)
#define PROFILER_FRAME_END()
#define PROFILER_SCOPE(name)

#endif

#endif // DIMX_CORE_PROFILER_H