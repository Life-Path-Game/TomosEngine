#pragma once

#include "../Event.hh"

namespace Tomos
{
    class KeyEvent : public Event
    {
    public:
        inline int  getKeyCode() const { return m_keyCode; }
        int getCategoryFlags() const override;

    protected:
        explicit KeyEvent( int p_keyCode ) : m_keyCode( p_keyCode ) {}

        int m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent( int p_keyCode, int p_repeatCount ) : KeyEvent( p_keyCode ), m_repeatCount( p_repeatCount ) {}

        inline int getRepeatCount() const { return m_repeatCount; }

        EventType   getEventType() const override { return EventType::KEY_PRESSED; }
        static EventType    getStaticType() { return EventType::KEY_PRESSED; }
        const char* getName() const override { return "KeyPressedEvent"; }
        std::string         toString() const override;

    protected:
        int m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent( int p_keyCode ) : KeyEvent( p_keyCode ) {}

        EventType   getEventType() const override { return EventType::KEY_RELEASED; }
        static EventType    getStaticType() { return EventType::KEY_RELEASED; }
        const char* getName() const override { return "KeyReleasedEvent"; }
        std::string         toString() const override;
    };

}  // namespace Tomos
