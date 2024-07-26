#pragma once
#define AL_LIBTYPE_STATIC
#include <audio/AudioPlayback.h>
#include <AL/al.h>

#define CHECK_AL_ERROR(msg)                                                        \
{                                                                                  \
    ALenum error = alGetError();                                                   \
    ASSERT(error == AL_NO_ERROR, "AL: " << msg << " FAILED with error=" << error); \
}

//    if (error != GL_NO_ERROR) LOGE("GL: " << msg << " FAILED with error=" << error);

#ifndef DIMX_PLATFORM_WEB
    #define SAFE_AL_CALL(call) call; CHECK_AL_ERROR(#call)
#else
    #define SAFE_AL_CALL(call) call;
#endif

ALenum fromCoreAudioFormat(AudioFormat coreFormat);
