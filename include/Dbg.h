#pragma once
#ifndef _DBG_H
#define _DBG_H

#include <spdlog/spdlog.h>
#include <memory>

#include "Core.h"

namespace sntl 
{
    class ENGINE_API Dbg
    {
    public:
        static Dbg* getInstance();
        std::shared_ptr<spdlog::logger>& getLogger() { return logger_; }

        ~Dbg();

        Dbg(const Dbg&) = delete;
        Dbg& operator=(const Dbg&) = delete;

    private:
        Dbg();

        static Dbg* instance_;
        std::shared_ptr<spdlog::logger> logger_;
    };
}

#define DBG_INFO(...)    SPDLOG_LOGGER_INFO(sntl::Dbg::getInstance()->getLogger(), __VA_ARGS__);
#define DBG_WARN(...)    SPDLOG_LOGGER_WARN(sntl::Dbg::getInstance()->getLogger(), __VA_ARGS__);
#define DBG_ERROR(...)   SPDLOG_LOGGER_ERROR(sntl::Dbg::getInstance()->getLogger(), __VA_ARGS__);
#define DBG_CRIT(...)    SPDLOG_LOGGER_CRITICAL(sntl::Dbg::getInstance()->getLogger(), __VA_ARGS__);

#endif