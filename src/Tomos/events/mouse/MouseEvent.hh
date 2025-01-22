#pragma once

#include "../Event.hh"

namespace Tomos
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent( double x, double y ) : x( x ), y( y ) {}

        inline double getX() const { return x; }
        inline double getY() const { return y; }

        virtual EventType   getEventType() const override { return EventType::MOUSE_MOVED; }
        virtual int         getCategoryFlags() const override;
        static EventType    getStaticType() { return EventType::MOUSE_MOVED; }
        virtual const char* getName() const override { return "MouseMovedEvent"; }
        std::string         toString() const override;

    private:
        double x, y;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent( double xOffset, double yOffset ) : xOffset( xOffset ), yOffset( yOffset ) {}

        inline double getXOffset() const { return xOffset; }
        inline double getYOffset() const { return yOffset; }

        virtual EventType   getEventType() const override { return EventType::MOUSE_SCROLLED; }
        virtual int         getCategoryFlags() const override;
        static EventType    getStaticType() { return EventType::MOUSE_SCROLLED; }
        virtual const char* getName() const override { return "MouseScrolledEvent"; }
        std::string         toString() const override;

    protected:
        double xOffset, yOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        inline int getButton() const { return button; }

        virtual int getCategoryFlags() const override;

    protected:
        MouseButtonEvent( int button ) : button( button ) {}

        int button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent( int button ) : MouseButtonEvent( button ) {}

        virtual EventType   getEventType() const override { return EventType::MOUSE_BUTTON_PRESSED; }
        static EventType    getStaticType() { return EventType::MOUSE_BUTTON_PRESSED; }
        virtual const char* getName() const override { return "MouseButtonPressedEvent"; }
        std::string         toString() const override;
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent( int button ) : MouseButtonEvent( button ) {}

        virtual EventType   getEventType() const override { return EventType::MOUSE_BUTTON_RELEASED; }
        static EventType    getStaticType() { return EventType::MOUSE_BUTTON_RELEASED; }
        virtual const char* getName() const override { return "MouseButtonReleasedEvent"; }
        std::string         toString() const override;
    };

}  // namespace Tomos
