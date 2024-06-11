#pragma once
#ifndef _CORE_H
#define _CORE_H

#include <csignal>
#include <cstdlib>

namespace sntl
{
    inline void killEngine(bool graceful)
    {
        graceful ? std::exit(-1) : std::abort();
    }
}

#define SNTL_KILL sntl::killEngine(false)

#ifdef WIN32
    #ifdef ENGINE_EXPORT
        #define ENGINE_API __declspec(dllexport)
    #else 
        #define ENGINE_API __declspec(dllimport)
    #endif

    #ifdef SNTL_DEBUG
        #define SNTL_BREAK __debugbreak()
    #else
        #define SNTL_BREAK sntl::killEngine(true)
    #endif

#else
    #define ENGINE_API

    #if defined(SNTL_DEBUG) && defined(SIGTRAP)
        #define SNTL_BREAK raise(SIGTRAP)
    #else 
        #define SNTL_BREAK sntl::killEngine(true)
    #endif
#endif

#endif