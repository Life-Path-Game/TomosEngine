#pragma once

#include <memory>
#include <unordered_map>

#include "Node.hh"
#include "Tomos/systems/System.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    class ECS
    {
    public:
        // Leave layer id -1 for systems that don't need to be in a specific layer and will be global
        template<typename T>
        void registerSystem( int p_layerId = -1 )
        {
            bool isBase = std::is_base_of_v<System, T>;
            LOG_ASSERT( isBase );

            if ( m_systems.contains( p_layerId ) && m_systems[p_layerId].contains( typeid( T ) ) )
            {
                LOG_ERROR() << "System already registered for this layer: " << typeid( T ).name();
                return;
            }

            m_systems[p_layerId][typeid( T )] = std::make_unique<T>();
            LOG_INFO() << "System registered: " << typeid( T ).name();
        }

        // Leave layer id -1 for systems that don't need to be in a specific layer and will be global
        template<typename T>
        void registerFixedTimeStepSystem( float p_fixedTimeStep = 0.01f, int p_layerId = -1 )
        {
            bool isBase = std::is_base_of_v<FixedTimeStepSystem, T>;
            LOG_ASSERT( isBase );

            if ( m_fixedTimeStepSystems.contains( p_layerId ) && m_fixedTimeStepSystems[p_layerId].contains( typeid( T ) ) )
            {
                LOG_ERROR() << "Fixed time step system already registered for this layer: " << typeid( T ).name();
                return;
            }

            m_fixedTimeStepSystems[p_layerId][typeid( T )] = std::make_unique<T>( p_fixedTimeStep );
            LOG_INFO() << "Fixed time step system registered: " << typeid( T ).name();
        }

        void registerComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node, int p_layerId = -1 );
        void destroyComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node, int p_layerId = -1 );

        void earlyUpdate( int p_layerId = -1 );
        void update( int p_layerId = -1 );
        void lateUpdate( int p_layerId = -1 );

        void updateFixedTimeStep( float p_deltaTime, int p_layerId = -1 );

        template<typename T>
        T& getSystem( int p_layerId = -1 )
        {
            if ( !m_systems.contains( p_layerId ) )
            {
                LOG_ERROR() << "Layer not found: " << p_layerId;
                return *static_cast<T*>( nullptr );
            }

            std::type_index type  = typeid( T );
            auto&           layer = m_systems[p_layerId];

            if ( layer.find( type ) == layer.end() )
            {
                LOG_ERROR() << "System not found: " << type.name();
            }
            return *static_cast<T*>( layer[type].get() );
        }

        template<typename T>
        T& getFixedTimeStepSystem( int p_layerId = -1 )
        {
            if ( !m_fixedTimeStepSystems.contains( p_layerId ) )
            {
                LOG_ERROR() << "Layer not found: " << p_layerId;
                return *static_cast<T*>( nullptr );
            }

            std::type_index type  = typeid( T );
            auto&           layer = m_fixedTimeStepSystems[p_layerId];

            if ( layer.find( type ) == layer.end() )
            {
                LOG_ERROR() << "Fixed time step system not found: " << type.name();
            }
            return *static_cast<T*>( layer[type].get() );
        }

    private:
        std::unordered_map<int, std::unordered_map<std::type_index, std::unique_ptr<System>>>              m_systems;
        std::unordered_map<int, std::unordered_map<std::type_index, std::unique_ptr<FixedTimeStepSystem>>> m_fixedTimeStepSystems;
    };
}  // namespace Tomos
