#pragma once

#include "../Event.hh"

namespace Tomos
{
    class KeyEvent : public Event
    {
    public:
        inline int  getKeyCode() const { return keyCode; }
        virtual int getCategoryFlags() const override;

    protected:
        KeyEvent( int keyCode ) : keyCode( keyCode ) {}

        int keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent( int keyCode, int repeatCount ) : KeyEvent( keyCode ), repeatCount( repeatCount ) {}

        inline int getRepeatCount() const { return repeatCount; }

        virtual EventType   getEventType() const override { return EventType::KEY_PRESSED; }
        static EventType    getStaticType() { return EventType::KEY_PRESSED; }
        virtual const char* getName() const override { return "KeyPressedEvent"; }
        std::string         toString() const override;

    protected:
        int repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent( int keyCode ) : KeyEvent( keyCode ) {}

        virtual EventType   getEventType() const override { return EventType::KEY_RELEASED; }
        static EventType    getStaticType() { return EventType::KEY_RELEASED; }
        virtual const char* getName() const override { return "KeyReleasedEvent"; }
        std::string         toString() const override;
    };

}  // namespace Tomos
