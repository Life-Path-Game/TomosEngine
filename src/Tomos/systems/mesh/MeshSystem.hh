#pragma once
#include <typeindex>

#include "MeshComponent.hh"
#include "Tomos/systems/System.hh"

namespace Tomos
{
    class MeshSystem : public System
    {
    public:
        struct DrawCall
        {
            std::shared_ptr<Shader>      m_shader;
            std::shared_ptr<Material>    m_material;
            std::shared_ptr<VertexArray> m_vertexArray;
            glm::mat4                    m_transform;
        };

        MeshSystem()
        {
        }

        std::type_index getComponentType() const override { return typeid( MeshComponent ); }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void lateUpdate( int p_layerId ) override;

        void                         clearDrawCalls( int layerId );
        const std::vector<DrawCall>& getDrawCalls( int layerId ) const;

    private:
        std::unordered_map<int, std::vector<DrawCall>> m_drawCalls;
    };
} // Tomos
