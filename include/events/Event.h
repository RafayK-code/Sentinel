#pragma once
#ifndef _EVENT_H
#define _EVENT_H

#include <string>
#include <functional>
#include <memory>

namespace sntl
{
    namespace 
    { 
        constexpr int NUM_EVENTS = 14; 
    }

    enum EventType
    {
        None = -1,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyDown, KeyUp, KeyTyped,
        MouseButtonDown, MouseButtonUp, MouseMoved, MouseScrolled
    };

    class IEvent 
    {
    public:
        virtual ~IEvent() = default;

        virtual EventType getEventType() const = 0;
        virtual std::string getName() const = 0;

        //exists if a handler would like to stop propogating the event further
        mutable bool handled = false;
    };

    template<typename E>
    using EventCallback = std::function<void(const E& e)>;

    using CallbackID = std::uint64_t;

    class EventDispatcher
    {
    public:
        EventDispatcher()
        {
            callbacks_.resize(NUM_EVENTS);
        }

        ~EventDispatcher()
        {
        }

        template<typename E>
        CallbackID addEventCallback(const EventCallback<E>& callback)
        {
            IEventCallback wrapper = [callback](const IEvent* e) 
            {
                if (auto* event = dynamic_cast<const E*>(e))
                {
                    callback(*event);
                }
            };

            callbacks_[E::getStaticEventType()].push_back(wrapper);
            return generateCallbackID<E>();
        }

        void removeEventCallback(CallbackID callbackID)
        {
            std::uint32_t index1 = callbackID >> 32;
            std::uint32_t index2 = (std::uint32_t)callbackID;

            callbacks_[index1].erase(callbacks_[index1].begin() + index2);
        }

    protected:
        template<typename E>
        void dispatchEvent(const E& e)
        {
            std::vector<IEventCallback>& eCallbacks = callbacks_[E::getStaticEventType()];
            for (const auto& callback : eCallbacks)
            {
                if (e.handled)
                    break;

                callback(&e);
            }
        }

    private:
        template<typename E>
        CallbackID generateCallbackID()
        {
            std::uint32_t low = callbacks_[E::getStaticEventType()].size() - 1;
            std::uint32_t high = E::getStaticEventType();

            return ((CallbackID)high << 32) | low;
        }

        using IEventCallback = std::function<void(const IEvent*)>;
        std::vector<std::vector<IEventCallback>> callbacks_;
    };
}

#endif