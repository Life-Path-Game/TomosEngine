#pragma once
#include "BillboardComponent.hh"
#include "Tomos/systems/System.hh"
#include "Tomos/util/renderer/Shader.hh"
#include "Tomos/util/renderer/Renderer.hh"

namespace Tomos
{
    class BillboardSystem : public System
    {
    public:
        BillboardSystem()
        {
        }

        std::type_index getComponentType() const override { return typeid( BillboardComponent ); }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void lateUpdate( int p_layerId ) override;

        void                         clearDrawCalls( int layerId );
        const std::vector<DrawCall>& getDrawCalls( int layerId ) const;

    private:
        std::unordered_map<int, std::vector<DrawCall>> m_drawCalls;
    };
} // Tomos
