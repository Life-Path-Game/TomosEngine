#pragma once
#include "ScriptComponent.hh"
#include "Tomos/systems/System.hh"

namespace Tomos
{
    class ScriptSystem : public System
    {
    public:
        ScriptSystem()
        {
        }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void earlyUpdate( int p_layerId ) override;
        void update( int p_layerId ) override;
        void lateUpdate( int p_layerId ) override;

        std::type_index getComponentType() const override { return typeid( ScriptComponent ); }
    };
} // Tomos
