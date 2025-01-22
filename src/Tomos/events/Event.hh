#pragma once

#include <functional>
#include <string>

namespace Tomos
{
    enum class EventType
    {
        NONE = 0,
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        WINDOW_FOCUS,
        WINDOW_LOST_FOCUS,
        WINDOW_MOVED,
        APP_TICK,
        APP_UPDATE,
        APP_RENDER,
        KEY_PRESSED,
        KEY_RELEASED,
        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        MOUSE_MOVED,
        MOUSE_SCROLLED
    };

    enum class EventCategory
    {
        NONE         = 0,
        APPLICATION  = 1 << 0,
        INPUT        = 1 << 1,
        KEYBOARD     = 1 << 2,
        MOUSE        = 1 << 3,
        MOUSE_BUTTON = 1 << 4
    };

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual EventType   getEventType() const     = 0;
        virtual int         getCategoryFlags() const = 0;
        virtual const char* getName() const          = 0;
        virtual std::string toString() const;
        static EventType    getStaticType() { return EventType::NONE; }

        inline bool isInCategory( EventCategory category ) const;
        inline bool isHandled() const { return handled; }

    protected:
        bool      handled = false;
        EventType type    = EventType::NONE;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool( T& )>;

    public:
        EventDispatcher( Event& event ) : event( event ) {}

        template<typename T>
        bool dispatch( EventFn<T> func )
        {
            if ( event.getEventType() == T::getStaticType() )
            {
                event.handled = func( *( T* ) &event );
                return true;
            }
            return false;
        }

    private:
        Event& event;
    };

}  // namespace Tomos
