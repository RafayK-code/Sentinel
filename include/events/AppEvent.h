#pragma once
#ifndef _APP_EVENT_H
#define _APP_EVENT_H

#include "Event.h"

namespace sntl
{
    class AppTickEvent : public IEvent
    {
    public:
        AppTickEvent(float dt)
            : dt_(dt)
        {
        }

        float getDt() const { return dt_; }

        EventType getEventType() const override { return EventType::AppTick; }
        static EventType getStaticEventType() { return EventType::AppTick; }
        std::string getName() const override { return "AppTickEvent"; }

    private:
        float dt_;
    };

    class AppUpdateEvent : public IEvent
    {
    public:
        AppUpdateEvent() = default;

        EventType getEventType() const override { return EventType::AppUpdate; }
        static EventType getStaticEventType() { return EventType::AppUpdate; }
        std::string getName() const override { return "AppUpdateEvent"; }
    };

    class AppRenderEvent : public IEvent
    {
    public:
        AppRenderEvent() = default;

        EventType getEventType() const override { return EventType::AppRender; }
        static EventType getStaticEventType() { return EventType::AppRender; }
        std::string getName() const override { return "AppRenderEvent"; }
    };
}

#endif