#pragma once
#include "ScriptComponent.hh"
#include "Tomos/systems/System.hh"

namespace Tomos
{
    class ScriptSystem : public System
    {
    public:
        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void earlyUpdate() override;
        void update() override;
        void lateUpdate() override;

        std::type_index getComponentType() const override { return typeid( ScriptComponent ); }
    };
} // Tomos
