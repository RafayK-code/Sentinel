#pragma once
#ifndef _KEY_EVENT_H
#define _KEY_EVENT_H

#include "Event.h"
#include "core/KeyCodes.h"

namespace sntl
{
    class KeyEvent : public IEvent
    {
    public:
        KeyCode getKeyCode() const { return keyCode_; }

    protected:
        KeyEvent(KeyCode keycode)
            : keyCode_(keycode)
        {
        }

        KeyCode keyCode_;
    };

    class KeyDownEvent : public KeyEvent
    {
    public:
        KeyDownEvent(KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), repeat_(isRepeat)
        {
        }

        bool isRepeat() const { return repeat_; }

        EventType getEventType() const override { return EventType::KeyDown; }
        static EventType getStaticEventType() { return EventType::KeyDown; }
        std::string getName() const override { return "KeyDownEvent"; }

    private:
        bool repeat_;
    };

    class KeyUpEvent : public KeyEvent
    {
    public:
        KeyUpEvent(KeyCode keycode)
            : KeyEvent(keycode)
        {
        }

        EventType getEventType() const override { return EventType::KeyUp; }
        static EventType getStaticEventType() { return EventType::KeyUp; }
        std::string getName() const override { return "KeyUpEvent"; }
    };

    class KeyTyped : public KeyEvent
    {
    public:
        KeyTyped(KeyCode keycode)
            : KeyEvent(keycode)
        {
        }

        EventType getEventType() const override { return EventType::KeyTyped; }
        static EventType getStaticEventType() { return EventType::KeyTyped; }
        std::string getName() const override { return "KeyTypedEvent"; }
    };
}

#endif