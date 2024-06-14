#pragma once
#ifndef _DBG_H
#define _DBG_H

#include <spdlog/spdlog.h>
#include <memory>

#include "Core.h"
#include "sys/Singleton.h"

namespace sntl 
{
    class ENGINE_API Dbg : public Singleton<Dbg>
    {
        friend class Singleton<Dbg>;
    public:
        std::shared_ptr<spdlog::logger>& getLogger() { return logger_; }
        ~Dbg();

    private:
        Dbg();

        std::shared_ptr<spdlog::logger> logger_;
    };

    void ENGINE_API initLogger();
    void ENGINE_API killLogger();
}

#ifdef SNTL_ENABLE_LOGGING
    #define DBG_INFO(...)    SPDLOG_LOGGER_INFO(sntl::Dbg::getPtr()->getLogger(), __VA_ARGS__)
    #define DBG_WARN(...)    SPDLOG_LOGGER_WARN(sntl::Dbg::getPtr()->getLogger(), __VA_ARGS__)
    #define DBG_ERROR(...)   SPDLOG_LOGGER_ERROR(sntl::Dbg::getPtr()->getLogger(), __VA_ARGS__)
    #define DBG_CRIT(...)    SPDLOG_LOGGER_CRITICAL(sntl::Dbg::getPtr()->getLogger(), __VA_ARGS__)
#else
    #define DBG_INFO(...)
    #define DBG_WARN(...)
    #define DBG_ERROR(...)
    #define DBG_CRIT(...)
#endif

// For now, asserts are always active as it implies program is in an unrecoverable state
#define DBG_ASSERT(cond, ...)  if(!cond) { SPDLOG_LOGGER_CRITICAL(sntl::Dbg::getInstance()->getLogger(), __VA_ARGS__); SNTL_BREAK; }

#endif