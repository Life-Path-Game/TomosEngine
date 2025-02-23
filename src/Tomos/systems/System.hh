#pragma once

#include <map>
#include <typeindex>

#include "Tomos/core/Node.hh"
#include "Component.hh"

namespace Tomos
{
    class System
    {
    public:
        virtual ~System() = default;

        virtual void earlyUpdate() {};
        virtual void update() {};
        virtual void lateUpdate() {};

        virtual void componentAdded( std::shared_ptr<Component> p_component, std::shared_ptr<Node> p_node ) {};
        virtual void componentRemoved( std::shared_ptr<Component> p_component, std::shared_ptr<Node> p_node ) {};

        virtual std::type_index getComponentType() const { return typeid( nullptr ); };

    protected:
        std::map<std::shared_ptr<Component>, std::shared_ptr<Node>> components;
    };
}  // namespace Tomos
