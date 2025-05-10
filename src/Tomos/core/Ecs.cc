//
// Created by dstuden on 1/20/25.
//

#include "Ecs.hh"

namespace Tomos
{
    void ECS::registerComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node, int p_layerId )
    {
        LOG_DEBUG() << "Start";
        auto t = typeid( *p_component ).hash_code();

        if ( m_systems.contains( p_layerId ) )
        {
            for ( auto& [_, system] : m_systems[p_layerId] )
            {
                if ( system->getComponentType().hash_code() == t )
                {
                    system->componentAdded( p_component, p_node );
                    LOG_DEBUG() << "End";
                    return;
                }
            }
        }

        if ( m_fixedTimeStepSystems.contains( p_layerId ) )
        {
            for ( auto& [_, system] : m_fixedTimeStepSystems[p_layerId] )
            {
                if ( system->getComponentType().hash_code() == t )
                {
                    system->componentAdded( p_component, p_node );
                    LOG_DEBUG() << "End";
                    return;
                }
            }
        }

        LOG_WARN() << "End: No system found for component: " << p_component->m_name;
    }

    void ECS::destroyComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node, int p_layerId )
    {
        LOG_DEBUG() << "Start";
        auto t = typeid( *p_component ).hash_code();

        if ( m_systems.contains( p_layerId ) )
        {
            for ( auto& [_, system] : m_systems[p_layerId] )
            {
                if ( system->getComponentType().hash_code() == t )
                {
                    system->componentRemoved( p_component, p_node );
                    LOG_DEBUG() << "End";
                    return;
                }
            }
        }

        if ( m_fixedTimeStepSystems.contains( p_layerId ) )
        {
            for ( auto& [_, system] : m_fixedTimeStepSystems[p_layerId] )
            {
                if ( system->getComponentType().hash_code() == t )
                {
                    system->componentRemoved( p_component, p_node );
                    LOG_DEBUG() << "End";
                    return;
                }
            }
        }

        LOG_WARN() << "No system found for component: " << p_component->m_name;
    }


    void ECS::earlyUpdate( int p_layerId )
    {
        if ( !m_systems.contains( p_layerId ) )
        {
            // No systems registered for this layer
            return;
        }

        for ( auto& [_, system] : m_systems[p_layerId] )
        {
            system->earlyUpdate();
        }
    }

    void ECS::update( int p_layerId )
    {
        if ( !m_systems.contains( p_layerId ) )
        {
            // No systems registered for this layer
            return;
        }

        for ( auto& [_, system] : m_systems[p_layerId] )
        {
            // No systems registered for this layer
            system->update();
        }
    }

    void ECS::lateUpdate( int p_layerId )
    {
        if ( !m_systems.contains( p_layerId ) )
        {
            // No systems registered for this layer
            return;
        }

        for ( auto& [_, system] : m_systems[p_layerId] )
        {
            system->lateUpdate();
        }
    }

    void ECS::updateFixedTimeStep( float p_deltaTime, int p_layerId )
    {
        if ( !m_fixedTimeStepSystems.contains( p_layerId ) )
        {
            // No systems registered for this layer
            return;
        }

        for ( auto& [_, system] : m_fixedTimeStepSystems[p_layerId] )
        {
            system->update( p_deltaTime );
        }
    }
}  // namespace Tomos
