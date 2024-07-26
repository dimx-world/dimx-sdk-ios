#pragma once

#ifndef DIMX_CORE_MEM_STREAM_H
#define DIMX_CORE_MEM_STREAM_H

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
struct IMemStreamBuf: std::streambuf
{
     IMemStreamBuf(const char* data, size_t size)
     {
         this->setg(const_cast<char*>(data),
                    const_cast<char*>(data),
                    const_cast<char*>(data + size));
     }

    size_t unreadBytes() const { return egptr() - gptr(); }
};

class IMemStream: private virtual IMemStreamBuf, public std::istream
{
public:
    IMemStream(const char* data, size_t size)
        : IMemStreamBuf(data, size)
        , std::ios(static_cast<std::streambuf*>(this))
        , std::istream(static_cast<std::streambuf*>(this))
    {
    }
    size_t unreadBytes() const { return IMemStreamBuf::unreadBytes(); }
};

////////////////////////////////////////////////////////////////////////////////

struct OMemStreamBuf: std::streambuf
{
   OMemStreamBuf(char* data, size_t size) 
   {
       setp(data, data + size);
   }
   size_t bytesWritten() const {return pptr() - pbase();}
};

class OMemStream: private virtual OMemStreamBuf, public std::ostream
{
public:
    OMemStream(char* data, size_t size)
        : OMemStreamBuf(data, size)
        , std::ostream(static_cast<std::streambuf*>(this))
    {
    }

    size_t bytesWritten() const { return OMemStreamBuf::bytesWritten(); }
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIMX_CORE_MEM_STREAM_H