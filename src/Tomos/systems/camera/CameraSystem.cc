//
// Created by dstuden on 1/20/25.
//

#include "CameraSystem.hh"

#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    void CameraSystem::componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        if ( !m_components.erase( p_component ) )
        {
            LOG_DEBUG() << "Component moved to new node: " << p_component->m_name;
        }
        m_components[p_component] = p_node;
    }

    void CameraSystem::componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        m_components.erase( p_component );
    }

    void CameraSystem::lateUpdate()
    {
        auto cam = std::find_if( m_components.begin(), m_components.end(),
                                 []( std::pair<std::shared_ptr<Component>, std::shared_ptr<Node>> p_pair )
                                 {
                                     auto c = std::dynamic_pointer_cast<CameraComponent>( p_pair.first );
                                     return ( c && c->m_active && p_pair.second->isActive() );
                                 } );

        if ( cam == m_components.end() )
        {
            LOG_WARN() << "No active camera found";
            return;
        }

        const auto cc  = std::dynamic_pointer_cast<CameraComponent>( cam->first );
        m_activeCamera = cc;
        const auto tc  = cam->second->m_transform;

        m_viewMat = tc.m_globInvMat;

        m_viewMatInv = tc.m_globMat;

        m_viewProjMat = cc->getProjection() * m_viewMat;

        m_viewProjMatInv = m_viewMatInv * cc->getInvProjection();
    }

    std::shared_ptr<Node> CameraSystem::getActiveCameraNode() const
    {
        auto c = std::dynamic_pointer_cast<Component>( m_activeCamera );
        if ( c )
        {
            auto n = m_components.find( c );
            if ( n != m_components.end() )
            {
                return n->second;
            }
        }

        return nullptr;
    }

    const std::shared_ptr<CameraComponent>& CameraSystem::getActiveCamera() const
    {
        return m_activeCamera;
    }

    const glm::mat4& CameraSystem::getViewProjectionMat() const
    {
        return m_viewProjMat;
    }

    const glm::mat4& CameraSystem::getViewProjectionInvMat() const
    {
        return m_viewProjMatInv;
    }

    const glm::mat4& CameraSystem::getViewMat() const
    {
        return m_viewMat;
    }

    const glm::mat4& CameraSystem::getViewInvMat() const
    {
        return m_viewMatInv;
    }
} // namespace Tomos
