//
// Created by dstuden on 5/31/25.
//

#include "BillboardSystem.hh"

#include "Tomos/core/Application.hh"
#include "Tomos/systems/camera/CameraSystem.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    void BillboardSystem::componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        auto meshComponent = std::dynamic_pointer_cast<BillboardComponent>( p_component );
        if ( m_components.contains( p_node->getLayerId() ) && !m_components[p_node->getLayerId()].erase( p_component ) )
        {
            LOG_DEBUG() << "Component moved to new node: " << p_component->m_name;
        }
        m_components[p_node->getLayerId()][p_component] = p_node;
    }

    void BillboardSystem::componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        if ( m_components.contains( p_node->getLayerId() ) )
            m_components[p_node->getLayerId()].erase( p_component );
    }



    void BillboardSystem::lateUpdate( int p_layerId )
    {
        auto sys      = Application::getState().ecs().getSystem<CameraSystem>();
        auto viewProj = sys.getViewProjectionMat( p_layerId );

        if ( !m_components.contains( p_layerId ) )
        {
            return;
        }

        m_drawCalls[p_layerId].clear();

        for ( auto& [component, node] : m_components[p_layerId] )
        {
            if ( !node->isActive() ) continue;

            auto billboardComponent = std::dynamic_pointer_cast<BillboardComponent>( component );
            billboardComponent->

            if ( mesh && material )
            {
                m_drawCalls[p_layerId].push_back( {
                        mesh->getShader(),
                        material,
                        mesh->getVertexArray(),
                        node->m_transform.m_globMat
                } );
            }
        }
    }

    const std::vector<DrawCall>& BillboardSystem::getDrawCalls( int layerId ) const
    {
        static const std::vector<DrawCall> empty;
        auto                               it = m_drawCalls.find( layerId );
        return it != m_drawCalls.end() ? it->second : empty;
    }

    void BillboardSystem::clearDrawCalls( int layerId )
    {
        m_drawCalls[layerId].clear();
    }
} // Tomos
