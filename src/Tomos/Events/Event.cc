//
// Created by dstuden on 1/19/25.
//

#include "Event.hh"

#include <sstream>

namespace Tomos
{
    std::string Event::toString() const
    {
        std::stringstream ss;
        ss << getName() << " ( " << static_cast<int>( type ) << " )";
        return ss.str();
    }

    bool Event::isInCategory( EventCategory category ) const
    {
        return getCategoryFlags() & static_cast<int>( category );
    }

}  // namespace Tomos
