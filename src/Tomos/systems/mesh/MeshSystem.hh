#pragma once
#include <typeindex>

#include "MeshComponent.hh"
#include "Tomos/systems/System.hh"

namespace Tomos
{
    class MeshSystem : public System
    {
    public:
        MeshSystem()
        {
        }

        std::type_index getComponentType() const override { return typeid( MeshComponent ); }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void lateUpdate( int p_layerId ) override;

    private:
    };
} // Tomos
