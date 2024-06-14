#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

#include "Core.h"
#include "Dbg.h"
#include "sys/Singleton.h"
#include "events/Event.h"

namespace sntl
{
    class ENGINE_API Engine : public Singleton<Engine>
    {
        friend class Singleton<Engine>;
    public:
        ~Engine() {}

        EventDispatcher& getDispatcher() { return globalDispatcher_; };

    private:
        Engine() {}

        EventDispatcher globalDispatcher_;
    };

    void ENGINE_API initEngine();
    void ENGINE_API killEngine();
}

#endif