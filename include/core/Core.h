#pragma once
#ifndef _CORE_H
#define _CORE_H

#include <csignal>
#include <cstdlib>

#if defined(SNTL_PLATFORM_WINDOWS) && defined(SNTL_SHARED_LIB)
    #ifdef SNTL_EXPORT
        #define SNTL_API __declspec(dllexport)
    #else 
        #define SNTL_API __declspec(dllimport)
    #endif
#else
    #define SNTL_API
#endif

namespace sntl
{
    void SNTL_API killEngine();

    inline void terminate(bool graceful)
    {
        if (graceful)
        {
            killEngine();
            std::exit(-1);
        }
        
        std::abort();
    }
}

#ifdef SNTL_PLATFORM_WINDOWS
    #ifdef SNTL_DEBUG
        #define SNTL_BREAK __debugbreak()
    #else
        #define SNTL_BREAK sntl::terminate(true)
    #endif

#else
    #if defined(SNTL_DEBUG) && defined(SIGTRAP)
        #define SNTL_BREAK raise(SIGTRAP)
    #else 
        #define SNTL_BREAK sntl::terminate(true)
    #endif
#endif

#endif