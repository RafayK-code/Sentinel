#pragma once
#ifndef _WINDOW_H
#define _WINDOW_H

#include "events/Event.h"

namespace sntl
{
    class Window : public EventDispatcher {
    public:
        virtual ~Window() = default;

        virtual void onTick() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual bool isVSync() const = 0;
        virtual void setVSync(bool enabled) = 0;

        virtual bool isFullScreen() const = 0;

        virtual void* getNativePtr() const = 0;
    };
}

#endif