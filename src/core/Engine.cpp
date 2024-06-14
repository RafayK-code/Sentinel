#include "core/Engine.h"

namespace sntl
{
    void initEngine()
    {
        Engine::createSingleton();
        initLogger();
    }

    void killEngine()
    {
        killLogger();
        Engine::killSingleton();
    }
}