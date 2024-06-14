#pragma once
#ifndef _MOUSE_EVENT_H
#define _MOUSE_EVENT_H

#include "Event.h"
#include "core/MouseCodes.h"

namespace sntl
{
    class MouseButtonEvent : public IEvent
    {
    public:
        MouseCode getMouseButton() const { return button_; }

    protected:
        MouseButtonEvent(MouseCode button)
            : button_(button)
        {
        }

        MouseCode button_;
    };

    class MouseButtonDownEvent : public MouseButtonEvent
    {
    public:
        MouseButtonDownEvent(MouseCode button)
            : MouseButtonEvent(button)
        {
        }

        EventType getEventType() const override { return EventType::MouseButtonDown; }
        static EventType getStaticEventType() { return EventType::MouseButtonDown; }
        std::string getName() const override { return "MouseButtonDownEvent"; }
    };

    class MouseButtonUpEvent : public MouseButtonEvent
    {
    public:
        MouseButtonUpEvent(MouseCode button)
            : MouseButtonEvent(button)
        {
        }

        EventType getEventType() const override { return EventType::MouseButtonUp; }
        static EventType getStaticEventType() { return EventType::MouseButtonUp; }
        std::string getName() const override { return "MouseButtonUpEvent"; }
    };

    class MouseMovedEvent : public IEvent
    {
    public:
        MouseMovedEvent(float x, float y)
            : mouseX_(x), mouseY_(y)
        {
        }

        float getX() const { return mouseX_; }
        float getY() const { return mouseY_; }

        EventType getEventType() const override { return EventType::MouseMoved; }
        static EventType getStaticEventType() { return EventType::MouseMoved; }
        std::string getName() const override { return "MouseMovedEvent"; }

    private:
        float mouseX_;
        float mouseY_;
    };

    class MouseScrolledEvent : public IEvent
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : xOffset_(xOffset), yOffset_(yOffset)
        {
        }

        float getXOffset() const { return xOffset_; }
        float getYOffset() const { return yOffset_; }

        EventType getEventType() const override { return EventType::MouseScrolled; }
        static EventType getStaticEventType() { return EventType::MouseScrolled; }
        std::string getName() const override { return "MouseScrolledEvent"; }

    private:
        float xOffset_;
        float yOffset_;
    };
}

#endif