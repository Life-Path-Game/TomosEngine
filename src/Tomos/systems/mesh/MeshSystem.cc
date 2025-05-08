//
// Created by dstuden on 4/1/25.
//

#include "MeshSystem.hh"

#include "Tomos/core/Application.hh"
#include "Tomos/systems/camera/CameraSystem.hh"
#include "Tomos/util/renderer/Renderer.hh"

namespace Tomos
{
    void MeshSystem::componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        auto meshComponent = std::dynamic_pointer_cast<MeshComponent>( p_component );
        if ( !m_components.erase( p_component ) )
        {
            LOG_WARN() << "Component moved to new node: " << p_component->m_name;
        }
        m_components[p_component] = p_node;
    }

    void MeshSystem::componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        m_components.erase( p_component );
    }

    void MeshSystem::lateUpdate()
    {
        for ( auto& [component, node] : m_components )
        {
            if ( !node->isActive() ) continue;

            auto meshComponent = std::dynamic_pointer_cast<MeshComponent>( component );
            auto mesh          = meshComponent->getMesh();
            auto material      = meshComponent->getMaterial();

            if ( mesh && material )
            {
                material->bind();

                Renderer::draw( meshComponent->getMesh()->getShader(),
                                mesh->getVertexArray(),
                                node->m_transform.m_globMat,
                                Application::getState().m_ecs.getSystem<CameraSystem>().getViewProjectionMat() );
            }
        }
    }
} // Tomos
