#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

#include "Core.h"
#include "Dbg.h"
#include "Singleton.h"
#include "events/Event.h"

namespace sntl
{
    class SNTL_API Engine : public Singleton<Engine>, public EventDispatcher
    {
        friend class Singleton<Engine>;
    public:
        ~Engine() {}

    private:
        Engine() {}
    };

    void SNTL_API initEngine();
    void SNTL_API killEngine();
}

#endif