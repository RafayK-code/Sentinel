#include "Dbg.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

namespace sntl
{
    Dbg* Dbg::instance_;

    Dbg* Dbg::getInstance()
    {
        if (!instance_)
            instance_ = new Dbg();

        return instance_;
    }

    Dbg::Dbg()
    {
        spdlog::set_pattern("[%10n] [%^%4!l%$] [%-20s:%5#] %v");
        logger_ = spdlog::stdout_color_mt("SENTINEL");
        logger_->set_level(spdlog::level::trace);
    }

    Dbg::~Dbg()
    {
        delete instance_;
    }
}