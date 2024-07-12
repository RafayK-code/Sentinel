#pragma once
#ifndef _WINDOW_H
#define _WINDOW_H

#include "events/Event.h"

namespace sntl
{
    enum class WindowType
    {
        WINDOWED,
        BORDERLESS,
        FULLSCREEN,
    };

    class Window : public EventDispatcher {
    public:
        virtual ~Window() = default;

        virtual void onTick() = 0;

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual int getXPos() const = 0;
        virtual int getYPos() const = 0;

        virtual bool isOpen() const = 0;

        virtual bool isVSync() const = 0;
        virtual void setVSync(bool enabled) = 0;
        
        virtual WindowType getWindowType() const = 0;
        virtual void setWindowType(WindowType windowType) = 0;

        virtual void* getNativePtr() const = 0;
    };
}

#endif