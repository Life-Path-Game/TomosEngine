//
// Created by dstuden on 1/20/25.
//

#include "CameraComponent.hh"

#include "../../core/Application.hh"

namespace Tomos
{

    CameraComponent::CameraComponent( float fov, float near, float far, const std::string& name ) :
        fov( fov ), near( near ), far( far ), dirty( true )
    {
        this->name = name;
        recompute();
    }

    void CameraComponent::setFov( float fov )
    {
        if ( this->fov != fov )
        {
            this->fov = fov;
            dirty     = true;
        }
    }

    void CameraComponent::setNear( float near )
    {
        if ( this->near != near )
        {
            this->near = near;
            dirty      = true;
        }
    }

    void CameraComponent::setFar( float far )
    {
        if ( this->far != far )
        {
            this->far = far;
            dirty     = true;
        }
    }

    float CameraComponent::getFov() const { return fov; }

    float CameraComponent::getNear() const { return near; }

    float CameraComponent::getFar() const { return far; }

    void CameraComponent::recompute()
    {
        projection    = glm::perspective( glm::radians( fov ), Application::get()->getState().aspectRatio, near, far );
        invProjection = glm::inverse( projection );
        aspectRatio   = Application::get()->getState().aspectRatio;
        invProjection = glm::inverse( projection );
        dirty         = false;
    }

    glm::mat4 CameraComponent::getProjection()
    {
        if ( dirty || Application::get()->getState().aspectRatio != aspectRatio )
        {
            recompute();
        }
        return projection;
    }

    glm::mat4 CameraComponent::getInvProjection()
    {
        if ( dirty || Application::get()->getState().aspectRatio != aspectRatio )
        {
            recompute();
        }
        return invProjection;
    }

}  // namespace Tomos
