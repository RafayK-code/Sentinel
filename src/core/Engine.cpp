#include "core/Engine.h"

namespace sntl
{
    void Engine::init()
    {
        Engine::createSingleton();
        Dbg::init();
    }

    void Engine::destroy()
    {
        Dbg::destroy();
        Engine::destroySingleton();
    }

    void initEngine()
    {
        Engine::init();
    }

    void destroyEngine()
    {
        Engine::destroy();
    }
}