#pragma once
#include <string>

namespace Tomos
{
    class Component
    {
    public:
        virtual ~Component() = default;

        std::string m_name{};
    };
} // namespace Tomos
