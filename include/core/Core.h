#pragma once
#ifndef _CORE_H
#define _CORE_H

#include <csignal>
#include <cstdlib>

#if defined(WIN32) && defined(ENGINE_DLL)
    #ifdef ENGINE_EXPORT
        #define ENGINE_API __declspec(dllexport)
    #else 
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #define ENGINE_API
#endif

namespace sntl
{
    void ENGINE_API killEngine();

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

#ifdef WIN32
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