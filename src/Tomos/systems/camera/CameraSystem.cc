//
// Created by dstuden on 1/20/25.
//

#include "CameraSystem.hh"

#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    void CameraSystem::componentAdded( std::shared_ptr<Component> component, std::shared_ptr<Node> node )
    {
        if ( !components.erase( component ) )
        {
            LOG_WARN() << "Component moved to new node";
        }
        components[component] = node;
    }

    void CameraSystem::componentRemoved( std::shared_ptr<Component> component, std::shared_ptr<Node> node )
    {
        components.erase( component );
    }


    void CameraSystem::lateUpdate()
    {
        auto cam = std::find_if( components.begin(), components.end(),
                                 []( std::pair<std::shared_ptr<Component>, std::shared_ptr<Node>> pair )
                                 {
                                     auto c = std::dynamic_pointer_cast<CameraComponent>( pair.first );
                                     return ( c && c->m_active && pair.second->isActive() );
                                 } );

        if ( cam == components.end() )
        {
            LOG_WARN() << "No active camera found";
            return;
        }

        auto cc        = std::dynamic_pointer_cast<CameraComponent>( cam->first );
        m_activeCamera = cc;
        auto tc        = cam->second->m_transform;

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
            auto n = components.find( c );
            if ( n != components.end() )
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
