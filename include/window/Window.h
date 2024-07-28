#pragma once
#ifndef _WINDOW_H
#define _WINDOW_H

#include <memory>

#include "events/Event.h"
#include "core/Core.h"

namespace sntl
{
    enum class WindowType
    {
        WINDOWED,
        BORDERLESS,
        FULLSCREEN,
    };

    class IWindow : public EventDispatcher {
    public:
        virtual ~IWindow() = default;

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

    RefPtr<IWindow> SNTL_API createWindowGlfw(const std::string& title, int xpos, int ypos, int width, int height, WindowType type = WindowType::WINDOWED, bool maximized = false);
}

#endif