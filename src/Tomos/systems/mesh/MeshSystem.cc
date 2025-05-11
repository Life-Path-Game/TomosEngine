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
        if ( m_components.contains( p_node->getLayerId() ) && !m_components[p_node->getLayerId()].erase( p_component ) )
        {
            LOG_DEBUG() << "Component moved to new node: " << p_component->m_name;
        }
        m_components[p_node->getLayerId()][p_component] = p_node;
    }

    void MeshSystem::componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        if ( m_components.contains( p_node->getLayerId() ) )
            m_components[p_node->getLayerId()].erase( p_component );
    }

    void MeshSystem::lateUpdate( int p_layerId )
    {
        // Get view projection matrix once
        auto sys = Application::getState().m_ecs.getSystem<CameraSystem>();
        auto viewProj = sys.getViewProjectionMat( p_layerId );

        if ( !m_components.contains( p_layerId ) )
        {
            return;
        }

        // Start batch
        Renderer::beginBatch();

        for ( auto& [component, node] : m_components[p_layerId] )
        {
            if ( !node->isActive() ) continue;

            auto meshComponent = std::dynamic_pointer_cast<MeshComponent>( component );
            auto mesh          = meshComponent->getMesh();
            auto material      = meshComponent->getMaterial();

            if ( mesh && material )
            {
                Renderer::addToBatch( mesh->getShader(),
                                      material,
                                      mesh->getVertexArray(),
                                      node->m_transform.m_globMat );
            }
        }

        // End batch and render everything
        Renderer::endBatch( viewProj );
    }
} // Tomos
