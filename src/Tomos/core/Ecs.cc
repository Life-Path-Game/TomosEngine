//
// Created by dstuden on 1/20/25.
//

#include "Ecs.hh"

namespace Tomos
{
    void ECS::registerComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        LOG_DEBUG() << "Start";
        auto t = typeid( *p_component ).hash_code();
        for ( auto& [_, system] : systems )
        {
            if ( system->getComponentType().hash_code() == t )
            {
                system->componentAdded( p_component, p_node );
                LOG_DEBUG() << "End";
                return;
            }
        }
        LOG_WARN() << "No system found for component: " << p_component->m_name;
    }

    void ECS::destroyComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        LOG_DEBUG() << "Start";
        auto t = typeid( *p_component ).hash_code();
        for ( auto& [_, system] : systems )
        {
            if ( system->getComponentType().hash_code() == t )
            {
                system->componentRemoved( p_component, p_node );
                LOG_DEBUG() << "End";
                return;
            }
        }
        LOG_WARN() << "No system found for component: " << p_component->m_name;
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
} // namespace Tomos
