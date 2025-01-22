//
// Created by dstuden on 1/20/25.
//

#include "Ecs.hh"

namespace Tomos
{
    void ECS::registerComponent( const std::shared_ptr<Component>& component, const std::shared_ptr<Node>& node )
    {
        auto t = typeid( *component ).hash_code();
        for ( auto& [_, system] : systems )
        {
            if ( system->getComponentType().hash_code() == t )
            {
                system->componentAdded( component, node );
                return;
            }
        }
        LOG_WARN() << "No system found for component: " << component->name;
    }

    void ECS::destroyComponent( const std::shared_ptr<Component>& component, const std::shared_ptr<Node>& node )
    {
        auto t = typeid( *component ).hash_code();
        for ( auto& [_, system] : systems )
        {
            if ( system->getComponentType().hash_code() == t )
            {
                system->componentRemoved( component, node );
                return;
            }
        }
    }


    void ECS::earlyUpdate()
    {
        for ( auto& [_, system] : systems )
        {
            system->earlyUpdate();
        }
    }

    void ECS::update()
    {
        for ( auto& [_, system] : systems )
        {
            system->update();
        }
    }

    void ECS::lateUpdate()
    {
        for ( auto& [_, system] : systems )
        {
            system->lateUpdate();
        }
    }

}  // namespace Tomos
