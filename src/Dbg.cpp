#include "Dbg.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace sntl 
{
    Dbg* Dbg::instance_ = nullptr;

    Dbg* Dbg::getInstance()
    {
        if (!instance_)
            instance_ = new Dbg();

        return instance_;
    }

    Dbg::Dbg()
    {
        spdlog::set_pattern("[%10n] [%^%4!l%$] [%-20s:%5#] %v");
        logger_ = spdlog::stdout_color_mt("Sentinel");
        logger_->set_level(spdlog::level::info);
    }

    Dbg::~Dbg()
    {
        delete instance_;
    }
}