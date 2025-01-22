//
// Created by dstuden on 1/19/25.
//

#include "MouseEvent.hh"

#include <sstream>

namespace Tomos
{

    int MouseMovedEvent::getCategoryFlags() const
    {
        return static_cast<int>( EventCategory::MOUSE ) | static_cast<int>( EventCategory::INPUT );
    }

    std::string MouseMovedEvent::toString() const
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << x << ", " << y;
        return ss.str();
    }

    int MouseScrolledEvent::getCategoryFlags() const
    {
        return static_cast<int>( EventCategory::MOUSE ) | static_cast<int>( EventCategory::INPUT );
    }

    std::string MouseScrolledEvent::toString() const
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << xOffset << ", " << yOffset;
        return ss.str();
    }

    int MouseButtonEvent::getCategoryFlags() const
    {
        return static_cast<int>( EventCategory::MOUSE ) | static_cast<int>( EventCategory::INPUT ) |
               static_cast<int>( EventCategory::MOUSE_BUTTON );
    }

    std::string MouseButtonPressedEvent::toString() const
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << button;
        return ss.str();
    }

    std::string MouseButtonReleasedEvent::toString() const
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << button;
        return ss.str();
    }

}  // namespace Tomos
