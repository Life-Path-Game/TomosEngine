//
// Created by dstuden on 1/20/25.
//

#include "CameraSystem.hh"

#include "../../util/logger/Logger.hh"

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
                                     return ( c && c->active );
                                 } );

        if ( cam == components.end() )
        {
            LOG_WARN() << "No active camera found";
            return;
        }

        auto cc = std::dynamic_pointer_cast<CameraComponent>( cam->first );
        auto tc = cam->second->transform;

        viewMtx = tc.globInvMat;

        viewMtxInv = tc.globMat;

        viewProjMtx = cc->getProjection() * viewMtx;

        viewProjMtxInv = viewMtxInv * cc->getInvProjection();
    }

    std::shared_ptr<Node> CameraSystem::getActiveCameraNode() const
    {
        auto c = std::dynamic_pointer_cast<Component>( activeCamera );
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

}  // namespace Tomos
