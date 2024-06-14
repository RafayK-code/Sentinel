#include "core/Dbg.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace sntl 
{
    Dbg::Dbg()
    {
        spdlog::set_pattern("[%10n] [%^%4!l%$] [%-20s:%5#] %v");
        logger_ = spdlog::stdout_color_mt("Sentinel");
        logger_->set_level(spdlog::level::info);
    }

    Dbg::~Dbg()
    {
    }

    void initLogger()
    {
        Dbg::createSingleton();
    }

    void killLogger()
    {
        Dbg::killSingleton();
    }
}