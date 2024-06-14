#pragma once
#ifndef _WINDOW_EVENT_H
#define _WINDOW_EVENT_H

#include "Event.h"

namespace sntl
{
    class WindowCloseEvent : public IEvent
    {
    public:
        WindowCloseEvent() = default;

        EventType getEventType() const override { return EventType::WindowClose; }
        static EventType getStaticEventType() { return EventType::WindowClose; }
        std::string getName() const override { return "WindowCloseEvent"; }
    };

    class WindowResizeEvent : public IEvent 
    {
    public:
        WindowResizeEvent(int width, int height)
            : width_(width), height_(height)
        {
        }

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

        EventType getEventType() const override { return EventType::WindowResize; }
        static EventType getStaticEventType() { return EventType::WindowResize; }
        std::string getName() const override { return "WindowResizeEvent"; }

    private:
        int width_;
        int height_;
    };

    class WindowFocusEvent : public IEvent
    {
    public:
        WindowFocusEvent() = default;

        EventType getEventType() const override { return EventType::WindowFocus; }
        static EventType getStaticEventType() { return EventType::WindowFocus; }
        std::string getName() const override { return "WindowFocusEvent"; }
    };

    class WindowLostFocusEvent : public IEvent
    {
    public:
        WindowLostFocusEvent() = default;

        EventType getEventType() const override { return EventType::WindowLostFocus; }
        static EventType getStaticEventType() { return EventType::WindowLostFocus; }
        std::string getName() const override { return "WindowLostFocusEvent"; }
    };

    class WindowMovedEvent : public IEvent
    {
    public:
        WindowMovedEvent(int xpos, int ypos)
            : xpos_(xpos), ypos_(ypos)
        {
        }

        int getXPos() const { return xpos_; }
        int getYPos() const { return ypos_; }

        EventType getEventType() const override { return EventType::WindowMoved; }
        static EventType getStaticEventType() { return EventType::WindowMoved; }
        std::string getName() const override { return "WindowMovedEvent"; }

    private:
        int xpos_;
        int ypos_;
    };
}

#endif