#pragma once

#include "../System.hh"
#include "CameraComponent.hh"

#include <glm/glm.hpp>

namespace Tomos
{
    class CameraSystem : public System
    {
    public:
        CameraSystem()
        {
        }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void lateUpdate( int p_layerId ) override;

        std::type_index getComponentType() const override { return typeid( CameraComponent ); }

        std::shared_ptr<Node> getActiveCameraNode( int p_layerId );

        const std::shared_ptr<CameraComponent>& getActiveCamera( int p_layerId );
        const glm::mat4&                        getViewProjectionMat( int p_layerId );
        const glm::mat4&                        getViewProjectionInvMat( int p_layerId );
        const glm::mat4&                        getViewMat( int p_layerId );
        const glm::mat4&                        getViewInvMat( int p_layerId );

        struct CameraData
        {
            std::shared_ptr<CameraComponent> m_activeCamera;
            glm::mat4                        m_viewProjMat;
            glm::mat4                        m_viewProjMatInv;
            glm::mat4                        m_viewMat;
            glm::mat4                        m_viewMatInv;
        };

    private:
        std::unordered_map<int, CameraData> m_layerActiveData;
        CameraData                          m_defaultCameraData;
    };
} // namespace Tomos
