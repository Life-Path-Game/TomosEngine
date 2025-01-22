#pragma once

#include "../System.hh"
#include "CameraComponent.hh"

namespace Tomos
{

    class CameraSystem : public System
    {
    public:
        void componentAdded( std::shared_ptr<Component> component, std::shared_ptr<Node> node ) override;
        void componentRemoved( std::shared_ptr<Component> component, std::shared_ptr<Node> node ) override;

        void lateUpdate() override;

        std::type_index getComponentType() const override { return typeid( CameraComponent ); }

        std::shared_ptr<Node> getActiveCameraNode() const;

        std::shared_ptr<CameraComponent> activeCamera;

        glm::mat4 viewProjMtx    = glm::mat4( 1.0f );
        glm::mat4 viewProjMtxInv = glm::mat4( 1.0f );
        glm::mat4 viewMtx        = glm::mat4( 1.0f );
        glm::mat4 viewMtxInv     = glm::mat4( 1.0f );
    };

}  // namespace Tomos
