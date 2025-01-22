#pragma once

#include <glm/glm.hpp>

#include "../Component.hh"

namespace Tomos
{

    class CameraComponent : public Component
    {
    public:
        CameraComponent( float fov = 45.0f, float near = 0.1f, float far = 100.0f, const std::string& name = "Camera" );

        void setFov( float fov );
        void setNear( float near );
        void setFar( float far );

        float getFov() const;
        float getNear() const;
        float getFar() const;

        glm::mat4 getProjection();
        glm::mat4 getInvProjection();

        bool active = true;

    protected:
        void recompute();

        glm::mat4 projection    = glm::mat4( 1.0f );
        glm::mat4 invProjection = glm::mat4( 1.0f );
        bool      dirty;
        float     fov;
        float     far;
        float     near;
        float     aspectRatio = 0.0f;
    };

}  // namespace Tomos
