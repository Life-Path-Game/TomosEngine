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
        ss << getName() << " ( " << static_cast<int>( m_type ) << " )";
        return ss.str();
    }

    bool Event::isInCategory( EventCategory p_category ) const
    {
        return getCategoryFlags() & static_cast<int>( p_category );
    }

}  // namespace Tomos
