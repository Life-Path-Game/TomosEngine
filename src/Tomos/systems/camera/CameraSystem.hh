#pragma once

#include "../System.hh"
#include "CameraComponent.hh"

#include <glm/glm.hpp>

namespace Tomos
{
    class CameraSystem : public System
    {
    public:
        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void lateUpdate() override;

        std::type_index getComponentType() const override { return typeid( CameraComponent ); }

        std::shared_ptr<Node> getActiveCameraNode() const;

        const std::shared_ptr<CameraComponent>& getActiveCamera() const;
        const glm::mat4&                        getViewProjectionMat() const;
        const glm::mat4&                        getViewProjectionInvMat() const;
        const glm::mat4&                        getViewMat() const;
        const glm::mat4&                        getViewInvMat() const;

    private:
        std::shared_ptr<CameraComponent> m_activeCamera;

        glm::mat4 m_viewProjMat    = glm::mat4( 1.0f );
        glm::mat4 m_viewProjMatInv = glm::mat4( 1.0f );
        glm::mat4 m_viewMat        = glm::mat4( 1.0f );
        glm::mat4 m_viewMatInv     = glm::mat4( 1.0f );
    };
} // namespace Tomos
