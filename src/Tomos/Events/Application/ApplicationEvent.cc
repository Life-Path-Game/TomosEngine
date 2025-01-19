//
// Created by dstuden on 1/19/25.
//

#include "ApplicationEvent.hh"

#include <sstream>

namespace Tomos
{
    std::string WindowResizeEvent::toString() const
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width << ", " << height;
        return ss.str();
    }

    std::string WindowCloseEvent::toString() const { return getName(); }

    std::string AppTickEvent::toString() const { return getName(); }

    std::string AppUpdateEvent::toString() const { return getName(); }

    std::string AppRenderEvent::toString() const { return getName(); }
}  // namespace Tomos
