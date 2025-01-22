#pragma once

#include "../Event.hh"

namespace Tomos
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent( int width, int height ) : width( width ), height( height ) {}

        inline unsigned int getWidth() const { return width; }
        inline unsigned int getHeight() const { return height; }

        virtual EventType   getEventType() const override { return EventType::WINDOW_RESIZE; }
        static EventType    getStaticType() { return EventType::WINDOW_RESIZE; }
        virtual const char* getName() const override { return "WindowResizeEvent"; }
        virtual int         getCategoryFlags() const override { return static_cast<int>( EventCategory::APPLICATION ); }
        std::string         toString() const override;

    private:
        unsigned int width, height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        virtual EventType   getEventType() const override { return EventType::WINDOW_CLOSE; }
        static EventType    getStaticType() { return EventType::WINDOW_CLOSE; }
        virtual const char* getName() const override { return "WindowCloseEvent"; }
        virtual int         getCategoryFlags() const override { return static_cast<int>( EventCategory::APPLICATION ); }
        std::string         toString() const override;
    };

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() {}

        virtual EventType   getEventType() const override { return EventType::APP_TICK; }
        static EventType    getStaticType() { return EventType::APP_TICK; }
        virtual const char* getName() const override { return "AppTickEvent"; }
        virtual int         getCategoryFlags() const override { return static_cast<int>( EventCategory::APPLICATION ); }
        std::string         toString() const override;
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() {}

        virtual EventType   getEventType() const override { return EventType::APP_UPDATE; }
        static EventType    getStaticType() { return EventType::APP_UPDATE; }
        virtual const char* getName() const override { return "AppUpdateEvent"; }
        virtual int         getCategoryFlags() const override { return static_cast<int>( EventCategory::APPLICATION ); }
        std::string         toString() const override;
    };

    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() {}

        virtual EventType   getEventType() const override { return EventType::APP_RENDER; }
        static EventType    getStaticType() { return EventType::APP_RENDER; }
        virtual const char* getName() const override { return "AppRenderEvent"; }
        virtual int         getCategoryFlags() const override { return static_cast<int>( EventCategory::APPLICATION ); }
        std::string         toString() const override;
    };

}  // namespace Tomos
