//
// Created by dstuden on 1/20/25.
//

#include "CameraComponent.hh"

#include "Tomos/core/Application.hh"

namespace Tomos
{
    CameraComponent::CameraComponent( float p_fov, float p_near, float p_far, const std::string& p_name ) :
        m_fov( p_fov ), m_near( p_near ), m_far( p_far ), m_dirty( true )
    {
        m_name = p_name;
        recompute();
    }

    void CameraComponent::setFov( float p_fov )
    {
        if ( m_fov != p_fov )
        {
            m_fov   = p_fov;
            m_dirty = true;
        }
    }

    void CameraComponent::setNear( float p_near )
    {
        if ( m_near != p_near )
        {
            m_near  = p_near;
            m_dirty = true;
        }
    }

    void CameraComponent::setFar( float p_far )
    {
        if ( m_far != p_far )
        {
            m_far   = p_far;
            m_dirty = true;
        }
    }

    float CameraComponent::getFov() const { return m_fov; }

    float CameraComponent::getNear() const { return m_near; }

    float CameraComponent::getFar() const { return m_far; }

    void CameraComponent::recompute()
    {
        m_projection    = glm::perspective( glm::radians( m_fov ), Application::getState().m_aspectRatio, m_near, m_far );
        m_aspectRatio   = Application::getState().m_aspectRatio;
        m_invProjection = glm::inverse( m_projection );
        m_dirty         = false;
    }

    glm::mat4 CameraComponent::getProjection()
    {
        if ( m_dirty || Application::getState().m_aspectRatio != m_aspectRatio )
        {
            recompute();
        }
        return m_projection;
    }

    glm::mat4 CameraComponent::getInvProjection()
    {
        if ( m_dirty || Application::getState().m_aspectRatio != m_aspectRatio )
        {
            recompute();
        }
        return m_invProjection;
    }
} // namespace Tomos
